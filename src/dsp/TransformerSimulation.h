#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

enum class TransformerType
{
    INPUT,
    OUTPUT
};

class TransformerSimulation
{
public:
    explicit TransformerSimulation (TransformerType type);
    ~TransformerSimulation() = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void process (juce::AudioBuffer<float>& buffer);

    void setType (int transformerIndex); // 0-2 for different transformer models

private:
    void applyTransformerCharacteristics (juce::AudioBuffer<float>& buffer);

    TransformerType type;
    int transformerIndex = 1;
    double sampleRate = 0.0;

    float prevOutput1 = 0.0f;
    float prevOutput2 = 0.0f;
};
