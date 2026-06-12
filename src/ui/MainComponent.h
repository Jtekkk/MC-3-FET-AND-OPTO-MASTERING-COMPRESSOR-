#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class MC3PluginAudioProcessor;
class CompressorPanel;
class EQPanel;
class ControlsPanel;

class MainComponent : public juce::Component
{
public:
    explicit MainComponent (MC3PluginAudioProcessor& processor);
    ~MainComponent() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    MC3PluginAudioProcessor& processor;

    std::unique_ptr<CompressorPanel> compressorPanel;
    std::unique_ptr<EQPanel> eqPanel;
    std::unique_ptr<ControlsPanel> controlsPanel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
