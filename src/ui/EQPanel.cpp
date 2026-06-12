#include "EQPanel.h"
#include "../PluginProcessor.h"

EQPanel::EQPanel (MC3PluginAudioProcessor& p)
    : processor (p)
    , lowGainAttach  (p.getAPVTS(), "eqLowGain",  lowGainSlider)
    , midGainAttach  (p.getAPVTS(), "eqMidGain",  midGainSlider)
    , highGainAttach (p.getAPVTS(), "eqHighGain", highGainSlider)
    , eqBypassAttach (p.getAPVTS(), "eqBypass",   eqBypassButton)
{
    addAndMakeVisible (lowGainSlider);
    addAndMakeVisible (midGainSlider);
    addAndMakeVisible (highGainSlider);
    addAndMakeVisible (eqBypassButton);
}

EQPanel::~EQPanel() {}

void EQPanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xFF1a1a1a));

    g.setColour (juce::Colours::cyan);
    g.setFont (juce::Font (13.0f, juce::Font::bold));
    g.drawText ("3-BAND EQ", 10, 6, 200, 18, juce::Justification::left);

    g.setColour (juce::Colour (0xFF444444));
    g.drawHorizontalLine (getHeight() - 1, 0.0f, (float) getWidth());
}

void EQPanel::resized()
{
    auto b = getLocalBounds().reduced (8);
    b.removeFromTop (26);

    int knobSz  = 70;
    int bypassH = 24;
    int gap     = 6;

    auto row = b.removeFromTop (knobSz);

    lowGainSlider.setBounds  (row.removeFromLeft (knobSz + gap).withHeight (knobSz));
    midGainSlider.setBounds  (row.removeFromLeft (knobSz + gap).withHeight (knobSz));
    highGainSlider.setBounds (row.removeFromLeft (knobSz + gap).withHeight (knobSz));

    eqBypassButton.setBounds (b.removeFromTop (bypassH).reduced (4, 0));
}
