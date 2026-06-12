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
    juce::ToggleButton oversamplingButton { "8X OVERSAMPLE" };
    juce::Slider       dryWetSlider     { juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight };
    juce::Slider       outputGainSlider { juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight };

    // Combo attachments are built after the items are added, so they are pointers.
    std::unique_ptr<APVTS::ComboBoxAttachment> inputTransformerAttach;
    std::unique_ptr<APVTS::ComboBoxAttachment> outputTransformerAttach;
    APVTS::ButtonAttachment   oversamplingAttach;
    APVTS::SliderAttachment   dryWetAttach;
    APVTS::SliderAttachment   outputGainAttach;

    juce::Rectangle<int> titleArea;
    juce::Rectangle<int> lblInTf, lblOutTf, lblMix, lblOut;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ControlsPanel)
};
