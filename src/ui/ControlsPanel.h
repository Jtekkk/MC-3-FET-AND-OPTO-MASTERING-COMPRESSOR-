#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class MC3PluginAudioProcessor;

class ControlsPanel : public juce::Component
{
public:
    explicit ControlsPanel (MC3PluginAudioProcessor& processor);
    ~ControlsPanel() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    MC3PluginAudioProcessor& processor;

    std::unique_ptr<juce::ComboBox> inputTransformerBox;
    std::unique_ptr<juce::ComboBox> outputTransformerBox;
    std::unique_ptr<juce::ToggleButton> oversamplingButton;
    std::unique_ptr<juce::Slider> outputGainSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlsPanel)
};
