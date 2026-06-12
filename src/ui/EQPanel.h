#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class MC3PluginAudioProcessor;

class EQPanel : public juce::Component
{
public:
    explicit EQPanel (MC3PluginAudioProcessor& processor);
    ~EQPanel() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    MC3PluginAudioProcessor& processor;

    std::unique_ptr<juce::Slider> lowGainSlider;
    std::unique_ptr<juce::Slider> midGainSlider;
    std::unique_ptr<juce::Slider> highGainSlider;
    std::unique_ptr<juce::ToggleButton> eqBypassButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQPanel)
};
