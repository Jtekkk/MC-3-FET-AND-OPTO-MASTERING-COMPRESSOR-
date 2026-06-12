#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "MC3LookAndFeel.h"

class MC3PluginAudioProcessor;
class CompressorPanel;
class EQPanel;
class ControlsPanel;
class MeterPanel;
class PresetPanel;

class MainComponent : public juce::Component
{
public:
    explicit MainComponent (MC3PluginAudioProcessor& processor);
    ~MainComponent() override;

    void paint (juce::Graphics& g) override;
    void paintOverChildren (juce::Graphics& g) override;
    void resized() override;

private:
    void paintHeader (juce::Graphics& g, juce::Rectangle<int> area);

    MC3LookAndFeel lookAndFeel;          // declared first so it outlives children
    MC3PluginAudioProcessor& processor;

    std::unique_ptr<PresetPanel>     presetPanel;
    std::unique_ptr<MeterPanel>      meterPanel;
    std::unique_ptr<CompressorPanel> compressorPanel;
    std::unique_ptr<EQPanel>         eqPanel;
    std::unique_ptr<ControlsPanel>   controlsPanel;

    juce::Rectangle<int> headerArea;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
