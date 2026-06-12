#include "TransformerSimulation.h"
#include "../utils/Utilities.h"

TransformerSimulation::TransformerSimulation (TransformerType type) : type (type)
{
}

void TransformerSimulation::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;
    prevOutput1 = 0.0f;
    prevOutput2 = 0.0f;
}

void TransformerSimulation::process (juce::AudioBuffer<float>& buffer)
{
    applyTransformerCharacteristics (buffer);
}

void TransformerSimulation::setType (int transformerIndex)
{
    this->transformerIndex = juce::jlimit (0, 2, transformerIndex);
}

void TransformerSimulation::applyTransformerCharacteristics (juce::AudioBuffer<float>& buffer)
{
    auto numChannels = buffer.getNumChannels();
    auto numSamples = buffer.getNumSamples();

    // Transformer characteristics vary by type
    // Type 0: Bright/Linear transformer
    // Type 1: Neutral/Balanced transformer
    // Type 2: Warm/Colored transformer

    float saturationAmount = 0.0f;
    float lowFreqBoost = 0.0f;
    float harmonic2ndAmount = 0.0f;

    switch (transformerIndex)
    {
        case 0: // Bright
            saturationAmount = 0.3f;
            lowFreqBoost = 0.0f;
            harmonic2ndAmount = 0.1f;
            break;
        case 1: // Neutral (default)
            saturationAmount = 0.5f;
            lowFreqBoost = 1.0f;
            harmonic2ndAmount = 0.15f;
            break;
        case 2: // Warm
            saturationAmount = 0.8f;
            lowFreqBoost = 3.0f;
            harmonic2ndAmount = 0.25f;
            break;
    }

    for (int ch = 0; ch < numChannels; ++ch)
    {
        auto* samples = buffer.getWritePointer (ch);
        for (int sample = 0; sample < numSamples; ++sample)
        {
            float input = samples[sample];

            // Soft saturation for transformer effect
            float saturated = MC3Utilities::softClip (input * saturationAmount) / (saturationAmount + 0.1f);

            // Add harmonic distortion
            float harmonic = saturated * harmonic2ndAmount * std::sin (saturated);

            samples[sample] = saturated + harmonic;
        }
    }
}
