#include "CompressorProcessor.h"
#include "../utils/Utilities.h"
#include <cmath>

CompressorProcessor::CompressorProcessor (CompressorType type) : type (type) {}

void CompressorProcessor::prepareToPlay (double sr, int)
{
    sampleRate = sr;
    envelope = 0.0f;
    gainReductionDb = 0.0f;
    updateCoefficients();
}

void CompressorProcessor::process (juce::AudioBuffer<float>& buffer)
{
    const int numChannels = buffer.getNumChannels();
    const int numSamples  = buffer.getNumSamples();

    const float makeupLinear = MC3Utilities::dbToLinear (makeupGain);

    for (int n = 0; n < numSamples; ++n)
    {
        float peak = 0.0f;
        for (int ch = 0; ch < numChannels; ++ch)
            peak = std::max (peak, std::abs (buffer.getSample (ch, n)));

        const float inputDb = MC3Utilities::linearToDb (peak);
        float targetGain = 1.0f;

        if (inputDb > threshold)
        {
            const float gainReduction = (threshold - inputDb) * (1.0f - 1.0f / ratio);
            targetGain = MC3Utilities::dbToLinear (gainReduction);
            envelope   = attackCoeff  * envelope + (1.0f - attackCoeff)  * targetGain;
        }
        else
        {
            envelope = releaseCoeff * envelope + (1.0f - releaseCoeff) * 1.0f;
        }

        gainReductionDb = MC3Utilities::linearToDb (envelope);

        for (int ch = 0; ch < numChannels; ++ch)
            buffer.setSample (ch, n, buffer.getSample (ch, n) * envelope * makeupLinear);
    }
}

void CompressorProcessor::setThreshold  (float db) { threshold  = db; }
void CompressorProcessor::setRatio      (float r)  { ratio      = r;  }
void CompressorProcessor::setMakeupGain (float db) { makeupGain = db; }

void CompressorProcessor::setAttack (float ms)
{
    attackMs = ms;
    updateCoefficients();
}

void CompressorProcessor::setRelease (float ms)
{
    releaseMs = ms;
    updateCoefficients();
}

void CompressorProcessor::updateCoefficients()
{
    if (sampleRate <= 0.0) return;
    attackCoeff  = MC3Utilities::timeConstantToCoefficient (attackMs,  (float) sampleRate);
    releaseCoeff = MC3Utilities::timeConstantToCoefficient (releaseMs, (float) sampleRate);
}
