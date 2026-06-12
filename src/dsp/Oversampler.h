#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>

class Oversampler
{
public:
    Oversampler (int factor, double originalSampleRate);
    ~Oversampler() = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock);

    juce::AudioBuffer<float>* upsample (juce::AudioBuffer<float>& inputBuffer);
    void downsample (juce::AudioBuffer<float>& oversampledBuffer, juce::AudioBuffer<float>& outputBuffer);

private:
    int factor;
    double originalSampleRate;
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampler;
    std::unique_ptr<juce::AudioBuffer<float>> oversampledBuffer;
};
