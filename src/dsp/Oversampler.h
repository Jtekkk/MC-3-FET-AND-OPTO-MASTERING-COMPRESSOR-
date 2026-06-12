#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_dsp/juce_dsp.h>
#include <vector>

class Oversampler
{
public:
    Oversampler (int factor, double originalSampleRate);
    ~Oversampler() = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock);

    // Upsamples the input and returns a buffer wrapping the oversampled data.
    juce::AudioBuffer<float>* upsample (juce::AudioBuffer<float>& inputBuffer);

    // Downsamples the internal oversampled data back into outputBuffer.
    void downsample (juce::AudioBuffer<float>& outputBuffer);

    float getLatencySamples() const;

private:
    int factor;
    double originalSampleRate;
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampler;

    // Non-owning wrapper around the oversampler's internal block (no per-block alloc)
    juce::AudioBuffer<float> wrapperBuffer;
    juce::dsp::AudioBlock<float> currentBlock;
    std::vector<float*> channelPointers;
};
