#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class LevelMeter;

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

class MeterPanel : public juce::Component
{
public:
    explicit MeterPanel (const LevelMeter& inputMeter, const LevelMeter& outputMeter);
    ~MeterPanel() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    std::unique_ptr<MeterDisplay> inputDisplay;
    std::unique_ptr<MeterDisplay> outputDisplay;
};
