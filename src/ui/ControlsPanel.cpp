#include "ControlsPanel.h"
#include "../PluginProcessor.h"

ControlsPanel::ControlsPanel (MC3PluginAudioProcessor& p)
    : processor (p)
    , inputTransformerAttach  (p.getAPVTS(), "inputTransformer",  inputTransformerBox)
    , outputTransformerAttach (p.getAPVTS(), "outputTransformer", outputTransformerBox)
    , oversamplingAttach      (p.getAPVTS(), "useOversampling",   oversamplingButton)
    , outputGainAttach        (p.getAPVTS(), "outputGain",        outputGainSlider)
{
    auto addTransformerItems = [](juce::ComboBox& box)
    {
        box.addItem ("Bright",  1);
        box.addItem ("Neutral", 2);
        box.addItem ("Warm",    3);
    };

    addTransformerItems (inputTransformerBox);
    addTransformerItems (outputTransformerBox);

    addAndMakeVisible (inputTransformerBox);
    addAndMakeVisible (outputTransformerBox);
    addAndMakeVisible (oversamplingButton);
    addAndMakeVisible (outputGainSlider);
}

ControlsPanel::~ControlsPanel() {}

void ControlsPanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xFF222222));

    g.setColour (juce::Colours::white.withAlpha (0.6f));
    g.setFont (11.0f);

    g.drawText ("INPUT TRANSFORMER",  10, 14, 140, 16, juce::Justification::left);
    g.drawText ("OUTPUT TRANSFORMER", 10, 54, 140, 16, juce::Justification::left);
    g.drawText ("OUTPUT GAIN",        10, 110, 140, 16, juce::Justification::left);
}

void ControlsPanel::resized()
{
    inputTransformerBox.setBounds  (155, 10,  160, 24);
    outputTransformerBox.setBounds (155, 50,  160, 24);
    oversamplingButton.setBounds   (350, 30,  160, 28);
    outputGainSlider.setBounds     (155, 100, 500, 30);
}
