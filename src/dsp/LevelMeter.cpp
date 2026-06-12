#include "LevelMeter.h"
#include "../utils/Utilities.h"
#include <cmath>

LevelMeter::LevelMeter() {}

void LevelMeter::prepareToPlay (double sr)
{
    sampleRate = sr;
    peakHoldSamples = (int) (sr * 0.5);  // 500ms hold time
    peakHoldCounter = 0;
    peakHoldLevel = 0.0f;
    reset();
}

void LevelMeter::process (const juce::AudioBuffer<float>& buffer)
{
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    float peak = 0.0f;
    bool clipping = false;

    for (int ch = 0; ch < numChannels; ++ch)
    {
        const auto* samples = buffer.getReadPointer (ch);
        for (int n = 0; n < numSamples; ++n)
        {
            const float absVal = std::abs (samples[n]);
            peak = std::max (peak, absVal);

            if (absVal >= 0.99f)  // Close to clipping
                clipping = true;
        }
    }

    const float peakDb = MC3Utilities::linearToDb (peak);
    currentLevelDb.store (peakDb, std::memory_order_relaxed);
    isClipping_.store (clipping, std::memory_order_relaxed);

    // Peak hold with decay
    if (peakDb > peakHoldLevel)
    {
        peakHoldLevel = peakDb;
        peakHoldCounter = peakHoldSamples;
    }
    else if (peakHoldCounter > 0)
    {
        peakHoldCounter -= numSamples;
    }
    else
    {
        // Decay at ~1 dB per 100ms
        peakHoldLevel -= (float) numSamples / (float) sampleRate * 10.0f;
        peakHoldLevel = std::max (peakHoldLevel, -80.0f);
    }

    peakLevelDb.store (peakHoldLevel, std::memory_order_relaxed);
}

void LevelMeter::reset() noexcept
{
    currentLevelDb.store (-80.0f, std::memory_order_relaxed);
    peakLevelDb.store (-80.0f, std::memory_order_relaxed);
    isClipping_.store (false, std::memory_order_relaxed);
    peakHoldCounter = 0;
    peakHoldLevel = -80.0f;
}
