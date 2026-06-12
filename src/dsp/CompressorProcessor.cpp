#include "CompressorProcessor.h"
#include "../utils/Utilities.h"
#include <cmath>
#include <algorithm>

CompressorProcessor::CompressorProcessor (CompressorType type) : type (type) {}

void CompressorProcessor::prepareToPlay (double sr, int samplesPerBlock)
{
    sampleRate = sr;
    envelope = 0.0f;
    gainReductionDb = 0.0f;

    // Pre-allocate lookahead buffer
    lookaheadBuffer.clear();
    lookaheadBuffer.resize (samplesPerBlock * 2, 0.0f);

    sidechainFilter.prepareToPlay (sr);
    updateCoefficientsIfNeeded();
}

float CompressorProcessor::calculateTimeConstant (float ms) noexcept
{
    // Apply type-specific multipliers for FET vs Opto character
    if (type == CompressorType::FET)
    {
        // FET: faster attack, faster release (more aggressive)
        return ms * FET_ATTACK_MULT;  // Used for both, attack uses this directly
    }
    else
    {
        // Opto: slower attack, slower release (smoother)
        return ms * OPTO_ATTACK_MULT;
    }
}

inline float CompressorProcessor::detectPeak (const juce::AudioBuffer<float>& buffer,
                                              int startSample, int endSample) noexcept
{
    const int numChannels = buffer.getNumChannels();
    float peak = 0.0f;

    for (int ch = 0; ch < numChannels; ++ch)
    {
        const auto* samples = buffer.getReadPointer (ch);
        for (int n = startSample; n < endSample; ++n)
        {
            float sample = samples[n];
            if (sidechainEnabled)
                sample = sidechainFilter.processSample (sample);
            peak = std::max (peak, std::abs (sample));
        }
    }

    return MC3Utilities::linearToDb (peak);
}

inline float CompressorProcessor::applyKnee (float gainDb, float knee) noexcept
{
    if (knee <= 0.0f)
        return gainDb;  // Hard knee

    // Soft knee: smooth the transition into compression
    // If we're close to threshold, apply a gentler slope
    const float kneeStart = threshold - knee;
    const float kneeEnd = threshold;

    if (gainDb >= kneeEnd)
        return gainDb;  // Above knee, full compression

    if (gainDb < kneeStart)
    {
        // Below knee start, apply reduced compression ratio
        const float kneeRange = knee;
        const float normalizedGain = (gainDb - kneeStart) / kneeRange;
        const float reducedRatio = 1.0f + (ratio - 1.0f) * (normalizedGain * normalizedGain);
        return kneeStart + (gainDb - kneeStart) * (1.0f - (1.0f - 1.0f / reducedRatio));
    }

    // Inside knee region
    const float kneeRange = kneeEnd - kneeStart;
    const float normalizedGain = (gainDb - kneeStart) / kneeRange;
    const float reducedRatio = 1.0f + (ratio - 1.0f) * (normalizedGain * normalizedGain);

    return kneeStart + (gainDb - kneeStart) * (1.0f - (1.0f - 1.0f / reducedRatio));
}

inline float CompressorProcessor::updateEnvelope (float peakDb) noexcept
{
    float targetGain = 1.0f;

    // Apply knee-adjusted threshold
    float adjustedThreshold = applyKnee (peakDb, kneeWidth);

    // attackCoeff/releaseCoeff already encode the FET vs Opto character: the
    // type-specific time-constant multipliers are baked in by
    // updateCoefficientsIfNeeded(). They MUST be used directly here. Multiplying
    // them by the type factor a second time can push the one-pole coefficient
    // above 1.0 (e.g. Opto release ≈ 0.999 × 1.5 ≈ 1.5), which makes the
    // envelope smoother an unstable filter: it diverges toward ±inf, the gain
    // becomes inf/NaN, and the whole output is silenced.
    if (adjustedThreshold > threshold)
    {
        const float gainReduction = (threshold - adjustedThreshold) * (1.0f - invRatio);
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
    kneeSmooth.setTargetValue (kneeWidthTarget);
    makeupGainSmooth.setTargetValue (makeupGainTarget);
    dryWetSmooth.setTargetValue (dryWetTarget);

    // If using lookahead, analyze future samples
    float lookaheadPeakDb = -80.0f;
    if (lookaheadSamples > 0)
    {
        // Add current buffer to lookahead
        for (int n = 0; n < numSamples; ++n)
        {
            float sample = 0.0f;
            for (int ch = 0; ch < numChannels; ++ch)
                sample = std::max (sample, std::abs (buffer.getSample (ch, n)));
            lookaheadBuffer.push_back (sample);
        }

        // Analyze future samples (up to lookahead distance)
        int analyzeSamples = std::min ((int) lookaheadBuffer.size(), lookaheadSamples);
        if (analyzeSamples > 0)
        {
            float peak = 0.0f;
            for (int i = 0; i < analyzeSamples; ++i)
                peak = std::max (peak, lookaheadBuffer[i]);
            lookaheadPeakDb = MC3Utilities::linearToDb (peak);
        }
    }

    // ── Process all samples ───────────────────────────────────────────────
    for (int n = 0; n < numSamples; ++n)
    {
        // Sample-smooth parameters
        threshold = thresholdSmooth.getNextValue();
        ratio = ratioSmooth.getNextValue();
        invRatio = 1.0f / std::max (ratio, 1.0f);
        kneeWidth = kneeSmooth.getNextValue();
        const float currentMakeupGain = makeupGainSmooth.getNextValue();
        const float currentDryWet = dryWetSmooth.getNextValue();
        const float makeupLinear = MC3Utilities::dbToLinear (currentMakeupGain);

        // Detect peak at current sample
        float peak = 0.0f;
        for (int ch = 0; ch < numChannels; ++ch)
            peak = std::max (peak, std::abs (buffer.getSample (ch, n)));
        float peakDb = MC3Utilities::linearToDb (peak);

        // Use lookahead peak if available
        if (lookaheadSamples > 0 && lookaheadPeakDb > peakDb)
            peakDb = lookaheadPeakDb;

        // Update envelope
        const float gain = updateEnvelope (peakDb);
        gainReductionDb = MC3Utilities::linearToDb (gain);

        const float sampleGain = gain * makeupLinear;

        // Apply with dry/wet mix
        for (int ch = 0; ch < numChannels; ++ch)
        {
            const float original = buffer.getSample (ch, n);
            const float compressed = original * sampleGain;
            const float mixed = original * (1.0f - currentDryWet) + compressed * currentDryWet;
            buffer.setSample (ch, n, mixed);
        }

        // Manage lookahead buffer
        if (lookaheadSamples > 0 && !lookaheadBuffer.empty())
            lookaheadBuffer.pop_front();
    }
}

void CompressorProcessor::setThreshold (float db)
{
    thresholdTarget = db;
}

void CompressorProcessor::setRatio (float r)
{
    ratioTarget = r;
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

void CompressorProcessor::setKneeWidth (float db)
{
    kneeWidthTarget = std::max (0.0f, db);
}

void CompressorProcessor::setLookahead (float ms)
{
    lookaheadMsTarget = ms;
    updateCoefficientsIfNeeded();
}

void CompressorProcessor::setDryWet (float mix)
{
    dryWetTarget = juce::jlimit (0.0f, 1.0f, mix);
}

void CompressorProcessor::setSidechainFreq (float hz)
{
    sidechainFreqTarget = juce::jlimit (20.0f, 20000.0f, hz);
    if (sidechainFreq != sidechainFreqTarget)
    {
        sidechainFreq = sidechainFreqTarget;
        sidechainFilter.setFrequency (sidechainFreq);
    }
}

void CompressorProcessor::setSidechainEnabled (bool enabled)
{
    sidechainEnabledTarget = enabled;
    if (sidechainEnabled != sidechainEnabledTarget)
    {
        sidechainEnabled = sidechainEnabledTarget;
        sidechainFilter.setEnabled (sidechainEnabled);
    }
}

void CompressorProcessor::updateCoefficientsIfNeeded()
{
    if (attackMsTarget == attackMs && releaseMsTarget == releaseMs &&
        lookaheadMsTarget * sampleRate * 0.001f == lookaheadSamples)
        return;

    attackMs = attackMsTarget;
    releaseMs = releaseMsTarget;
    lookaheadSamples = (int) (lookaheadMsTarget * sampleRate * 0.001f);

    if (sampleRate <= 0.0) return;

    // Apply type-specific multipliers
    float attackConstant = calculateTimeConstant (attackMs);
    float releaseConstant = (type == CompressorType::FET)
        ? releaseMs * FET_RELEASE_MULT
        : releaseMs * OPTO_RELEASE_MULT;

    attackCoeff  = MC3Utilities::timeConstantToCoefficient (attackConstant,  (float) sampleRate);
    releaseCoeff = MC3Utilities::timeConstantToCoefficient (releaseConstant, (float) sampleRate);
}
