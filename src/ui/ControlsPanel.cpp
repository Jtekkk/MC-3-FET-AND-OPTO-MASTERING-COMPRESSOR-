#include "ControlsPanel.h"
#include "MC3LookAndFeel.h"
#include "../PluginProcessor.h"

using namespace mc3::colours;

ControlsPanel::ControlsPanel (MC3PluginAudioProcessor& p)
    : processor (p)
    , oversamplingAttach      (p.getAPVTS(), "useOversampling",   oversamplingButton)
    , dryWetAttach            (p.getAPVTS(), "dryWetMix",         dryWetSlider)
    , outputGainAttach        (p.getAPVTS(), "outputGain",        outputGainSlider)
{
    auto addItems = [] (juce::ComboBox& box)
    {
        box.addItem ("Bright",  1);
        box.addItem ("Neutral", 2);
        box.addItem ("Warm",    3);
    };
    addItems (inputTransformerBox);
    addItems (outputTransformerBox);

    // Attach AFTER items exist so the current parameter value selects an item.
    inputTransformerAttach  = std::make_unique<APVTS::ComboBoxAttachment> (
        p.getAPVTS(), "inputTransformer",  inputTransformerBox);
    outputTransformerAttach = std::make_unique<APVTS::ComboBoxAttachment> (
        p.getAPVTS(), "outputTransformer", outputTransformerBox);

    dryWetSlider.textFromValueFunction   = [] (double v) { return juce::String (juce::roundToInt (v * 100.0)) + " %"; };
    dryWetSlider.valueFromTextFunction   = [] (const juce::String& t) { return t.getDoubleValue() / 100.0; };
    outputGainSlider.textFromValueFunction = [] (double v) { return juce::String (v, 1) + " dB"; };
    outputGainSlider.valueFromTextFunction = [] (const juce::String& t) { return t.getDoubleValue(); };
    dryWetSlider.updateText();
    outputGainSlider.updateText();
    dryWetSlider.setColour (juce::Slider::textBoxTextColourId, amberBright);
    outputGainSlider.setColour (juce::Slider::textBoxTextColourId, amberBright);

    addAndMakeVisible (inputTransformerBox);
    addAndMakeVisible (outputTransformerBox);
    addAndMakeVisible (oversamplingButton);
    addAndMakeVisible (dryWetSlider);
    addAndMakeVisible (outputGainSlider);
}

ControlsPanel::~ControlsPanel() = default;

void ControlsPanel::paint (juce::Graphics& g)
{
    mc3::drawBrushedMetal (g, getLocalBounds().toFloat(), faceTop, faceBottom);

    auto plate = titleArea.toFloat();
    mc3::drawBrushedMetal (g, plate, panelRaised, juce::Colour (0xFF202024), 4.0f);
    g.setColour (amber.withAlpha (0.6f));
    g.drawRoundedRectangle (plate.reduced (0.5f), 4.0f, 1.0f);
    mc3::drawEngravedText (g, "OUTPUT STAGE", titleArea.withTrimmedLeft (12),
                           juce::Justification::centredLeft,
                           MC3LookAndFeel::engravedFont (15.0f, true), textLight);

    auto label = [&] (const juce::String& t, juce::Rectangle<int> r)
    {
        mc3::drawEngravedText (g, t, r, juce::Justification::centredLeft,
                               MC3LookAndFeel::engravedFont (11.0f, true), textDim);
    };
    label ("INPUT XFMR",  lblInTf);
    label ("OUTPUT XFMR", lblOutTf);
    label ("DRY / WET",   lblMix);
    label ("OUTPUT",      lblOut);
}

void ControlsPanel::resized()
{
    auto area = getLocalBounds().reduced (12, 10);
    titleArea = area.removeFromTop (26);
    area.removeFromTop (10);

    const int labelW = 96;
    const int rowH = 30;
    const int gap = 10;

    auto row = [&] () { auto r = area.removeFromTop (rowH); area.removeFromTop (gap); return r; };

    // Transformers (two combos side by side on one row)
    {
        auto r = row();
        auto left = r.removeFromLeft (r.getWidth() / 2);
        lblInTf = left.removeFromLeft (labelW);
        inputTransformerBox.setBounds (left.reduced (2, 3));

        r.removeFromLeft (gap);
        lblOutTf = r.removeFromLeft (labelW);
        outputTransformerBox.setBounds (r.reduced (2, 3));
    }

    // Dry/Wet
    {
        auto r = row();
        lblMix = r.removeFromLeft (labelW);
        dryWetSlider.setBounds (r.reduced (2, 2));
    }

    // Output gain
    {
        auto r = row();
        lblOut = r.removeFromLeft (labelW);
        outputGainSlider.setBounds (r.reduced (2, 2));
    }

    // Oversampling toggle
    oversamplingButton.setBounds (area.removeFromTop (30).reduced (2, 2).withSizeKeepingCentre (200, 26));
}
