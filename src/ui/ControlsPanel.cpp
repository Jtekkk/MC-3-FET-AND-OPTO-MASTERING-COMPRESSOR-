#include "ControlsPanel.h"
#include "../PluginProcessor.h"

ControlsPanel::ControlsPanel (MC3PluginAudioProcessor& p)
    : processor (p)
    , inputTransformerAttach  (p.getAPVTS(), "inputTransformer",  inputTransformerBox)
    , outputTransformerAttach (p.getAPVTS(), "outputTransformer", outputTransformerBox)
    , oversamplingAttach      (p.getAPVTS(), "useOversampling",   oversamplingButton)
    , dryWetAttach            (p.getAPVTS(), "dryWetMix",         dryWetSlider)
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
    addAndMakeVisible (dryWetSlider);
    addAndMakeVisible (outputGainSlider);
}

ControlsPanel::~ControlsPanel() {}

void ControlsPanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xFF222222));

    g.setColour (juce::Colours::white.withAlpha (0.6f));
    g.setFont (11.0f);

    g.drawText ("INPUT TRANSFORMER",  10, 6, 140, 16, juce::Justification::left);
    g.drawText ("OUTPUT TRANSFORMER", 10, 36, 140, 16, juce::Justification::left);
    g.drawText ("DRY/WET MIX",        10, 66, 140, 16, juce::Justification::left);
    g.drawText ("OUTPUT GAIN",        10, 96, 140, 16, juce::Justification::left);
}

void ControlsPanel::resized()
{
    inputTransformerBox.setBounds  (155, 4,   160, 24);
    outputTransformerBox.setBounds (155, 34,  160, 24);
    dryWetSlider.setBounds         (155, 64,  500, 22);
    oversamplingButton.setBounds   (350, 34,  140, 24);
    outputGainSlider.setBounds     (155, 94,  500, 22);
}
