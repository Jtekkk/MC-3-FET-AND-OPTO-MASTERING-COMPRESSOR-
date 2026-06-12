#include "Oversampler.h"

Oversampler::Oversampler (int factor, double originalSampleRate)
    : factor (factor), originalSampleRate (originalSampleRate)
{
}

void Oversampler::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    oversampler = std::make_unique<juce::dsp::Oversampling<float>> (2, juce::roundToInt (std::log2 (factor)), juce::dsp::Oversampling<float>::FilterType::filterHalfBandPolyphaseIIR);

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock * factor;
    spec.numChannels = 2;

    oversampler->prepare (spec);

    oversampledBuffer = std::make_unique<juce::AudioBuffer<float>> (2, samplesPerBlock * factor);
}

juce::AudioBuffer<float>* Oversampler::upsample (juce::AudioBuffer<float>& inputBuffer)
{
    juce::dsp::AudioBlock<float> inputBlock (inputBuffer);
    auto oversampledBlock = oversampler->processSamplesUp (inputBlock);
    oversampledBlock.copyTo (*oversampledBuffer);
    return oversampledBuffer.get();
}

void Oversampler::downsample (juce::AudioBuffer<float>& oversampledBuffer, juce::AudioBuffer<float>& outputBuffer)
{
    juce::dsp::AudioBlock<float> oversampledBlock (oversampledBuffer);
    auto downsampledBlock = oversampler->processSamplesDown (oversampledBlock);
    downsampledBlock.copyTo (outputBuffer);
}
