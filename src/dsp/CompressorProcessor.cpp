#include "CompressorProcessor.h"
#include "../utils/Utilities.h"
#include <cmath>

CompressorProcessor::CompressorProcessor (CompressorType type) : type (type)
{
}

void CompressorProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    envelope.assign (samplesPerBlock, 0.0f);
    updateCoefficients();
}

void CompressorProcessor::process (juce::AudioBuffer<float>& buffer)
{
    auto numChannels = buffer.getNumChannels();
    auto numSamples = buffer.getNumSamples();

    float makeupGainLinear = MC3Utilities::dbToLinear (makeupGain);

    for (int sample = 0; sample < numSamples; ++sample)
    {
        float maxInput = 0.0f;
        for (int ch = 0; ch < numChannels; ++ch)
        {
            maxInput = std::max (maxInput, std::abs (buffer.getSample (ch, sample)));
        }

        float inputDb = MC3Utilities::linearToDb (maxInput);

        if (inputDb > threshold)
        {
            float gainReduction = (threshold - inputDb) * (1.0f - 1.0f / ratio);
            gainReduction = MC3Utilities::dbToLinear (gainReduction);

            envelope[sample] = attackCoeff * envelope[sample] + (1.0f - attackCoeff) * gainReduction;
        }
        else
        {
            envelope[sample] = releaseCoeff * envelope[sample] + (1.0f - releaseCoeff) * 1.0f;
        }

        for (int ch = 0; ch < numChannels; ++ch)
        {
            buffer.setSample (ch, sample, buffer.getSample (ch, sample) * envelope[sample] * makeupGainLinear);
        }
    }
}

void CompressorProcessor::setThreshold (float db)
{
    threshold = db;
}

void CompressorProcessor::setRatio (float ratio)
{
    this->ratio = ratio;
}

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

void CompressorProcessor::setMakeupGain (float db)
{
    makeupGain = db;
}

void CompressorProcessor::updateCoefficients()
{
    if (sampleRate <= 0.0)
        return;

    attackCoeff = MC3Utilities::timeConstantToCoefficient (attackMs, sampleRate);
    releaseCoeff = MC3Utilities::timeConstantToCoefficient (releaseMs, sampleRate);
}
