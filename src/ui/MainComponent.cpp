#include "MainComponent.h"
#include "CompressorPanel.h"
#include "EQPanel.h"
#include "ControlsPanel.h"
#include "../PluginProcessor.h"

MainComponent::MainComponent (MC3PluginAudioProcessor& processor) : processor (processor)
{
    compressorPanel = std::make_unique<CompressorPanel> (processor);
    eqPanel = std::make_unique<EQPanel> (processor);
    controlsPanel = std::make_unique<ControlsPanel> (processor);

    addAndMakeVisible (compressorPanel.get());
    addAndMakeVisible (eqPanel.get());
    addAndMakeVisible (controlsPanel.get());
}

MainComponent::~MainComponent()
{
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xFF2a2a2a));
}

void MainComponent::resized()
{
    auto bounds = getLocalBounds();
    auto width = bounds.getWidth();

    compressorPanel->setBounds (0, 0, width / 2, 300);
    eqPanel->setBounds (width / 2, 0, width / 2, 300);
    controlsPanel->setBounds (0, 300, width, 300);
}
