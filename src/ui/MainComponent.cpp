#include "MainComponent.h"
#include "CompressorPanel.h"
#include "EQPanel.h"
#include "ControlsPanel.h"
#include "MeterComponent.h"
#include "../PluginProcessor.h"

MainComponent::MainComponent (MC3PluginAudioProcessor& processor) : processor (processor)
{
    compressorPanel = std::make_unique<CompressorPanel> (processor);
    eqPanel = std::make_unique<EQPanel> (processor);
    controlsPanel = std::make_unique<ControlsPanel> (processor);
    meterPanel = std::make_unique<MeterPanel> (processor.getInputMeter(), processor.getOutputMeter());

    addAndMakeVisible (compressorPanel.get());
    addAndMakeVisible (eqPanel.get());
    addAndMakeVisible (controlsPanel.get());
    addAndMakeVisible (meterPanel.get());
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
    auto height = bounds.getHeight();

    const int meterHeight = 100;
    const int controlsHeight = 180;
    const int processHeight = height - meterHeight - controlsHeight;

    meterPanel->setBounds (0, 0, width, meterHeight);

    auto procBounds = bounds.withTop (meterHeight).withHeight (processHeight);
    compressorPanel->setBounds (procBounds.removeFromLeft (width / 2));
    eqPanel->setBounds (procBounds);

    controlsPanel->setBounds (0, meterHeight + processHeight, width, controlsHeight);
}
