#include "SidechainFilter.h"

SidechainFilter::SidechainFilter() : hpFilter (std::make_unique<juce::dsp::IIR::Filter<float>>()) {}

void SidechainFilter::prepareToPlay (double sr)
{
    sampleRate = sr;
    setFrequency (frequency);
}

float SidechainFilter::processSample (float sample) noexcept
{
    if (!enabled || !hpFilter)
        return sample;

    return hpFilter->processSample (sample);
}

void SidechainFilter::setFrequency (float freqHz)
{
    frequency = freqHz;

    if (sampleRate <= 0.0 || !hpFilter)
        return;

    *hpFilter->coefficients = *juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, frequency);
}
