#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <functional>

class LevelMeter;
class CompressorProcessor;

// A backlit analogue-style VU meter with a swinging needle.
class VuMeter : public juce::Component, private juce::Timer
{
public:
    VuMeter (juce::String label, float minDb, float maxDb, bool reverse,
             float redZoneNorm, std::function<float()> valueProvider,
             bool enablePeakHold = false);
    ~VuMeter() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    void timerCallback() override;
    void renderFaceplate();
    float valueToAngle (float db) const;

    juce::String label;
    float minDb, maxDb;
    bool  reverse;
    float redZoneNorm;             // 0..1 where the red arc begins
    std::function<float()> provider;
    bool  peakHoldEnabled = false;

    float currentAngle = 0.0f;     // smoothed needle angle (radians from vertical)
    float targetAngle  = 0.0f;
    bool  overload = false;
    float peakHoldAngle = angleLeft;
    int   peakHoldCounter = 0;

    juce::Image faceplate;         // cached static background
    juce::Rectangle<float> windowBounds;  // inner lit window (for glass reflection)
    juce::Point<float> pivot;
    float needleLen = 0.0f;

    static constexpr float angleLeft  = -0.62f;
    static constexpr float angleRight =  0.62f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VuMeter)
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
    std::unique_ptr<VuMeter> inputMeter;
    std::unique_ptr<VuMeter> fetGr;
    std::unique_ptr<VuMeter> optoGr;
    std::unique_ptr<VuMeter> outputMeter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeterPanel)
};
