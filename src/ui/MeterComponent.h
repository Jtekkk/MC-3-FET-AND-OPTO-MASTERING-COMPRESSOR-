#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class LevelMeter;
class CompressorProcessor;

class MeterDisplay : public juce::Component, private juce::Timer
{
public:
    explicit MeterDisplay (const LevelMeter& meter, const juce::String& label);
    ~MeterDisplay() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override;

    const LevelMeter& meter;
    juce::String label;
    float displayPeakDb = -80.0f;
    bool  displayClipping = false;

    static constexpr int METER_MIN_DB = -60;
    static constexpr int METER_MAX_DB = 0;
};

class GainReductionMeter : public juce::Component, private juce::Timer
{
public:
    GainReductionMeter (const CompressorProcessor& compressor, const juce::String& label);
    ~GainReductionMeter() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override;

    const CompressorProcessor& compressor;
    juce::String label;
    float displayGrDb = 0.0f;

    static constexpr int GR_MIN_DB = -30;
    static constexpr int GR_MAX_DB = 0;
};

class MeterPanel : public juce::Component
{
public:
    MeterPanel (const LevelMeter& inputMeter, const LevelMeter& outputMeter,
                const CompressorProcessor& fetComp, const CompressorProcessor& optoComp);
    ~MeterPanel() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    std::unique_ptr<MeterDisplay> inputDisplay;
    std::unique_ptr<MeterDisplay> outputDisplay;
    std::unique_ptr<GainReductionMeter> fetGrMeter;
    std::unique_ptr<GainReductionMeter> optoGrMeter;
};
