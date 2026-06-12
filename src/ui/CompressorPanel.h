#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "LabeledKnob.h"

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

    struct Strip
    {
        juce::String title;
        juce::Colour tint;
        std::unique_ptr<LabeledKnob> threshold, ratio, attack, release, makeup, knee, lookahead, scFreq;
        juce::ToggleButton bypass { "BYPASS" };
        juce::ToggleButton sidechain { "SC HPF" };
        std::unique_ptr<APVTS::ButtonAttachment> bypassAttach, scAttach;
        juce::Rectangle<int> bounds, titleArea;
    };

    void buildStrip (Strip&, const juce::String& prefix, const juce::String& title, juce::Colour tint);
    void layoutStrip (Strip&, juce::Rectangle<int> area);
    void paintStrip (juce::Graphics&, Strip&);

    MC3PluginAudioProcessor& processor;
    Strip fet, opto;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CompressorPanel)
};
