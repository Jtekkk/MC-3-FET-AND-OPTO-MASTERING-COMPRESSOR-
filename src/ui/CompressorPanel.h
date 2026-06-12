#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
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
    using APVTS = juce::AudioProcessorValueTreeState;

    MC3PluginAudioProcessor& processor;

    // FET Compressor widgets
    juce::Slider fetThresholdSlider  { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::Slider fetRatioSlider      { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::Slider fetAttackSlider     { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::Slider fetReleaseSlider    { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::Slider fetMakeupGainSlider { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::Slider fetKneeSlider       { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::Slider fetLookaheadSlider  { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::ToggleButton fetBypassButton { "Bypass" };

    // Opto Compressor widgets
    juce::Slider optoThresholdSlider  { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::Slider optoRatioSlider      { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::Slider optoAttackSlider     { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::Slider optoReleaseSlider    { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::Slider optoMakeupGainSlider { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::Slider optoKneeSlider       { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::Slider optoLookaheadSlider  { juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow };
    juce::ToggleButton optoBypassButton { "Bypass" };

    // Attachments — must be declared after widgets so widgets outlive them
    APVTS::SliderAttachment fetThresholdAttach;
    APVTS::SliderAttachment fetRatioAttach;
    APVTS::SliderAttachment fetAttackAttach;
    APVTS::SliderAttachment fetReleaseAttach;
    APVTS::SliderAttachment fetMakeupGainAttach;
    APVTS::SliderAttachment fetKneeAttach;
    APVTS::SliderAttachment fetLookaheadAttach;
    APVTS::ButtonAttachment fetBypassAttach;

    APVTS::SliderAttachment optoThresholdAttach;
    APVTS::SliderAttachment optoRatioAttach;
    APVTS::SliderAttachment optoAttackAttach;
    APVTS::SliderAttachment optoReleaseAttach;
    APVTS::SliderAttachment optoMakeupGainAttach;
    APVTS::SliderAttachment optoKneeAttach;
    APVTS::SliderAttachment optoLookaheadAttach;
    APVTS::ButtonAttachment optoBypassAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorPanel)
};
