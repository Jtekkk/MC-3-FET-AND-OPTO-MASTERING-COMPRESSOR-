#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "LabeledKnob.h"

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

    std::unique_ptr<LabeledKnob> low, mid, high;
    juce::ToggleButton bypass { "BYPASS" };
    std::unique_ptr<APVTS::ButtonAttachment> bypassAttach;

    juce::Rectangle<int> titleArea;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQPanel)
};
