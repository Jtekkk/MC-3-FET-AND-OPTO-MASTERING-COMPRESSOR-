#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
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
    using APVTS = juce::AudioProcessorValueTreeState;

    MC3PluginAudioProcessor& processor;

    juce::Slider       lowGainSlider  { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::Slider       midGainSlider  { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::Slider       highGainSlider { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::ToggleButton eqBypassButton { "Bypass" };

    // Attachments after widgets
    APVTS::SliderAttachment lowGainAttach;
    APVTS::SliderAttachment midGainAttach;
    APVTS::SliderAttachment highGainAttach;
    APVTS::ButtonAttachment eqBypassAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQPanel)
};
