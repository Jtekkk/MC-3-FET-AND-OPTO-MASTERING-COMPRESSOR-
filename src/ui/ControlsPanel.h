#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
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
    using APVTS = juce::AudioProcessorValueTreeState;

    MC3PluginAudioProcessor& processor;

    juce::ComboBox     inputTransformerBox;
    juce::ComboBox     outputTransformerBox;
    juce::ToggleButton oversamplingButton { "8x Oversampling" };
    juce::Slider       outputGainSlider   { juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight };

    // Attachments after widgets
    APVTS::ComboBoxAttachment inputTransformerAttach;
    APVTS::ComboBoxAttachment outputTransformerAttach;
    APVTS::ButtonAttachment   oversamplingAttach;
    APVTS::SliderAttachment   outputGainAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlsPanel)
};
