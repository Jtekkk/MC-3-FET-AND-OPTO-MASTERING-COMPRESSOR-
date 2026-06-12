#include "CompressorProcessor.h"
#include "../utils/Utilities.h"
#include <cmath>
#include <algorithm>

CompressorProcessor::CompressorProcessor (CompressorType type) : type (type) {}

void CompressorProcessor::prepareToPlay (double sr, int)
{
    sampleRate = sr;
    envelope = 0.0f;
    gainReductionDb = 0.0f;
    updateCoefficientsIfNeeded();
}

inline float CompressorProcessor::detectPeak (const juce::AudioBuffer<float>& buffer) noexcept
{
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();
    float peak = 0.0f;

    for (int ch = 0; ch < numChannels; ++ch)
    {
        const auto* samples = buffer.getReadPointer (ch);
        for (int n = 0; n < numSamples; ++n)
            peak = std::max (peak, std::abs (samples[n]));
    }

    return MC3Utilities::linearToDb (peak);
}

inline float CompressorProcessor::updateEnvelope (float peakDb) noexcept
{
    float targetGain = 1.0f;

    if (peakDb > threshold)
    {
        const float gainReduction = (threshold - peakDb) * (1.0f - invRatio);
        targetGain = MC3Utilities::dbToLinear (gainReduction);
        envelope = attackCoeff * envelope + (1.0f - attackCoeff) * targetGain;
    }
    else
    {
        envelope = releaseCoeff * envelope + (1.0f - releaseCoeff) * 1.0f;
    }

    return envelope;
}

void CompressorProcessor::process (juce::AudioBuffer<float>& buffer)
{
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    // ── Update smoothed parameters ────────────────────────────────────────
    thresholdSmooth.setTargetValue (thresholdTarget);
    ratioSmooth.setTargetValue (ratioTarget);
    makeupGainSmooth.setTargetValue (makeupGainTarget);

    // Process all samples
    for (int n = 0; n < numSamples; ++n)
    {
        // Sample-smooth critical parameters to avoid clicks
        threshold = thresholdSmooth.getNextValue();
        ratio = ratioSmooth.getNextValue();
        const float currentMakeupGain = makeupGainSmooth.getNextValue();
        const float makeupLinear = MC3Utilities::dbToLinear (currentMakeupGain);

        // Detect peak in this sample across all channels
        float peak = 0.0f;
        for (int ch = 0; ch < numChannels; ++ch)
            peak = std::max (peak, std::abs (buffer.getSample (ch, n)));

        const float peakDb = MC3Utilities::linearToDb (peak);
        const float gain = updateEnvelope (peakDb);
        gainReductionDb = MC3Utilities::linearToDb (gain);

        // Apply compressed & gained sample
        const float sampleGain = gain * makeupLinear;
        for (int ch = 0; ch < numChannels; ++ch)
            buffer.setSample (ch, n, buffer.getSample (ch, n) * sampleGain);
    }
}

void CompressorProcessor::setThreshold (float db)
{
    thresholdTarget = db;
}

void CompressorProcessor::setRatio (float r)
{
    ratioTarget = r;
    invRatio = 1.0f / std::max (r, 1.0f);
}

void CompressorProcessor::setAttack (float ms)
{
    attackMsTarget = ms;
    updateCoefficientsIfNeeded();
}

void CompressorProcessor::setRelease (float ms)
{
    releaseMsTarget = ms;
    updateCoefficientsIfNeeded();
}

void CompressorProcessor::setMakeupGain (float db)
{
    makeupGainTarget = db;
}

void CompressorProcessor::updateCoefficientsIfNeeded()
{
    if (attackMsTarget == attackMs && releaseMsTarget == releaseMs)
        return;

    attackMs = attackMsTarget;
    releaseMs = releaseMsTarget;

    if (sampleRate <= 0.0) return;

    attackCoeff  = MC3Utilities::timeConstantToCoefficient (attackMs,  (float) sampleRate);
    releaseCoeff = MC3Utilities::timeConstantToCoefficient (releaseMs, (float) sampleRate);
}
