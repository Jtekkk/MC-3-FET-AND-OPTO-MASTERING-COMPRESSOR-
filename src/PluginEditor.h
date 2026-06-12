#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "PluginProcessor.h"
#include "ui/MainComponent.h"

class MC3PluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit MC3PluginAudioProcessorEditor (MC3PluginAudioProcessor&);
    ~MC3PluginAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    MC3PluginAudioProcessor& audioProcessor;
    std::unique_ptr<MainComponent> mainComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MC3PluginAudioProcessorEditor)
};
