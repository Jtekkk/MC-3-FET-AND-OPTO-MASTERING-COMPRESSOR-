#pragma once

#include <juce_dsp/juce_dsp.h>

class SidechainFilter
{
public:
    SidechainFilter();
    ~SidechainFilter() = default;

    void prepareToPlay (double sampleRate);
    float processSample (float sample) noexcept;
    void setFrequency (float freqHz);
    void setEnabled (bool shouldEnable) noexcept { enabled = shouldEnable; }

private:
    std::unique_ptr<juce::dsp::IIR::Filter<float>> hpFilter;
    double sampleRate = 0.0;
    float frequency = 20.0f;
    bool enabled = false;
};
