#include "Oversampler.h"
#include <cmath>

Oversampler::Oversampler (int factor, double originalSampleRate)
    : factor (factor), originalSampleRate (originalSampleRate)
{
}

void Oversampler::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    originalSampleRate = sampleRate;

    // JUCE's Oversampling takes the number of 2x stages, i.e. log2(factor).
    const size_t numStages = (size_t) juce::roundToInt (std::log2 ((double) factor));

    oversampler = std::make_unique<juce::dsp::Oversampling<float>> (
        2, numStages,
        juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR);

    oversampler->initProcessing ((size_t) samplesPerBlock);
    oversampler->reset();

    // Pre-size the channel pointer array to avoid allocations on the audio thread.
    channelPointers.assign (2, nullptr);
}

juce::AudioBuffer<float>* Oversampler::upsample (juce::AudioBuffer<float>& inputBuffer)
{
    juce::dsp::AudioBlock<float> inputBlock (inputBuffer);
    currentBlock = oversampler->processSamplesUp (inputBlock);

    const int numCh   = (int) currentBlock.getNumChannels();
    const int numSamp = (int) currentBlock.getNumSamples();

    if ((int) channelPointers.size() < numCh)
        channelPointers.resize ((size_t) numCh, nullptr);

    for (int ch = 0; ch < numCh; ++ch)
        channelPointers[(size_t) ch] = currentBlock.getChannelPointer ((size_t) ch);

    // Wrap the oversampler's internal storage without copying or allocating.
    wrapperBuffer.setDataToReferTo (channelPointers.data(), numCh, numSamp);
    return &wrapperBuffer;
}

void Oversampler::downsample (juce::AudioBuffer<float>& outputBuffer)
{
    juce::dsp::AudioBlock<float> outputBlock (outputBuffer);
    oversampler->processSamplesDown (outputBlock);
}

float Oversampler::getLatencySamples() const
{
    return oversampler != nullptr ? (float) oversampler->getLatencyInSamples() : 0.0f;
}
