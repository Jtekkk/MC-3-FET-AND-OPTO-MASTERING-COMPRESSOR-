#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

class EQProcessor
{
public:
    EQProcessor();
    ~EQProcessor() = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void process (juce::AudioBuffer<float>& buffer);

    void setLowGain (float db);
    void setMidGain (float db);
    void setHighGain (float db);

private:
    void updateLowShelf();
    void updateMidPeak();
    void updateHighShelf();

    double sampleRate = 0.0;

    float lowGain = 0.0f;
    float midGain = 0.0f;
    float highGain = 0.0f;

    using Filter = juce::dsp::IIR::Filter<float>;
    juce::dsp::ProcessorDuplicator<Filter, juce::dsp::IIR::Coefficients<float>> lowShelf;
    juce::dsp::ProcessorDuplicator<Filter, juce::dsp::IIR::Coefficients<float>> midPeak;
    juce::dsp::ProcessorDuplicator<Filter, juce::dsp::IIR::Coefficients<float>> highShelf;
};
