#include "PluginEditor.h"

MC3PluginAudioProcessorEditor::MC3PluginAudioProcessorEditor (MC3PluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    mainComponent = std::make_unique<MainComponent> (audioProcessor);
    addAndMakeVisible (mainComponent.get());

    setSize (1080, 720);
    setResizable (false, false);
}

MC3PluginAudioProcessorEditor::~MC3PluginAudioProcessorEditor()
{
}

void MC3PluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::darkgrey);
}

void MC3PluginAudioProcessorEditor::resized()
{
    mainComponent->setBounds (getLocalBounds());
}
