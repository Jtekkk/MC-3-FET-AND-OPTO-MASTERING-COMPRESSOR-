#include "MainComponent.h"
#include "PresetPanel.h"
#include "CompressorPanel.h"
#include "EQPanel.h"
#include "ControlsPanel.h"
#include "MeterComponent.h"
#include "../PluginProcessor.h"

MainComponent::MainComponent (MC3PluginAudioProcessor& processor) : processor (processor)
{
    presetPanel = std::make_unique<PresetPanel> (processor.getPresetManager());
    meterPanel = std::make_unique<MeterPanel> (processor.getInputMeter(), processor.getOutputMeter(),
                                                processor.getFetCompressor(), processor.getOptoCompressor());
    compressorPanel = std::make_unique<CompressorPanel> (processor);
    eqPanel = std::make_unique<EQPanel> (processor);
    controlsPanel = std::make_unique<ControlsPanel> (processor);

    addAndMakeVisible (presetPanel.get());
    addAndMakeVisible (meterPanel.get());
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
    auto height = bounds.getHeight();

    const int presetHeight = 40;
    const int meterHeight = 100;
    const int controlsHeight = 140;
    const int processHeight = height - presetHeight - meterHeight - controlsHeight;

    presetPanel->setBounds (0, 0, width, presetHeight);
    meterPanel->setBounds (0, presetHeight, width, meterHeight);

    auto procBounds = bounds.withTop (presetHeight + meterHeight).withHeight (processHeight);
    compressorPanel->setBounds (procBounds.removeFromLeft (width / 2));
    eqPanel->setBounds (procBounds);

    controlsPanel->setBounds (0, presetHeight + meterHeight + processHeight, width, controlsHeight);
}
