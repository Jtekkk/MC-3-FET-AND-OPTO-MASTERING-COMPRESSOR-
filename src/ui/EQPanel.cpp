#include "EQPanel.h"
#include "MC3LookAndFeel.h"
#include "../PluginProcessor.h"

using namespace mc3::colours;

EQPanel::EQPanel (MC3PluginAudioProcessor& p) : processor (p)
{
    auto& apvts = processor.getAPVTS();
    low  = std::make_unique<LabeledKnob> (apvts, "eqLowGain",  "LOW",  " dB");
    mid  = std::make_unique<LabeledKnob> (apvts, "eqMidGain",  "MID",  " dB");
    high = std::make_unique<LabeledKnob> (apvts, "eqHighGain", "HIGH", " dB");

    addAndMakeVisible (*low);
    addAndMakeVisible (*mid);
    addAndMakeVisible (*high);
    addAndMakeVisible (bypass);

    bypassAttach = std::make_unique<APVTS::ButtonAttachment> (apvts, "eqBypass", bypass);
}

EQPanel::~EQPanel() = default;

void EQPanel::paint (juce::Graphics& g)
{
    mc3::drawBrushedMetal (g, getLocalBounds().toFloat(), faceTop, faceBottom);

    auto plate = titleArea.toFloat();
    mc3::drawBrushedMetal (g, plate, panelRaised, juce::Colour (0xFF202024), 4.0f);
    g.setColour (amber.withAlpha (0.6f));
    g.drawRoundedRectangle (plate.reduced (0.5f), 4.0f, 1.0f);

    auto lamp = juce::Rectangle<float> (9.0f, 9.0f).withCentre ({ plate.getX() + 14.0f, plate.getCentreY() });
    g.setColour (amber.withAlpha (0.5f)); g.fillEllipse (lamp.expanded (4.0f));
    g.setColour (amber);                  g.fillEllipse (lamp);

    mc3::drawEngravedText (g, "EQUALISER", titleArea.withTrimmedLeft (28),
                           juce::Justification::centredLeft,
                           MC3LookAndFeel::engravedFont (15.0f, true), textLight);

    g.setColour (juce::Colours::black.withAlpha (0.4f));
    g.drawVerticalLine (getWidth() - 1, 8.0f, (float) getHeight() - 8.0f);
}

void EQPanel::resized()
{
    auto area = getLocalBounds().reduced (12, 10);
    titleArea = area.removeFromTop (26);
    area.removeFromTop (8);

    bypass.setBounds (area.removeFromBottom (28).reduced (4, 2).withSizeKeepingCentre (130, 24));
    area.removeFromBottom (6);

    const int colW = area.getWidth() / 3;
    low->setBounds  (area.removeFromLeft (colW).reduced (6));
    mid->setBounds  (area.removeFromLeft (colW).reduced (6));
    high->setBounds (area.reduced (6));
}
