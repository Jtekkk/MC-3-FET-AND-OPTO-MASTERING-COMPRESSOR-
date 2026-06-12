#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class MC3PluginAudioProcessor;

class CompressorPanel : public juce::Component
{
public:
    explicit CompressorPanel (MC3PluginAudioProcessor& processor);
    ~CompressorPanel() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    MC3PluginAudioProcessor& processor;

    // FET Compressor Controls
    std::unique_ptr<juce::Slider> fetThresholdSlider;
    std::unique_ptr<juce::Slider> fetRatioSlider;
    std::unique_ptr<juce::Slider> fetAttackSlider;
    std::unique_ptr<juce::Slider> fetReleaseSlider;
    std::unique_ptr<juce::Slider> fetMakeupGainSlider;
    std::unique_ptr<juce::ToggleButton> fetBypassButton;

    // Opto Compressor Controls
    std::unique_ptr<juce::Slider> optoThresholdSlider;
    std::unique_ptr<juce::Slider> optoRatioSlider;
    std::unique_ptr<juce::Slider> optoAttackSlider;
    std::unique_ptr<juce::Slider> optoReleaseSlider;
    std::unique_ptr<juce::Slider> optoMakeupGainSlider;
    std::unique_ptr<juce::ToggleButton> optoBypassButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorPanel)
};
