#include "CompressorPanel.h"
#include "../PluginProcessor.h"

CompressorPanel::CompressorPanel (MC3PluginAudioProcessor& p)
    : processor (p)
    , fetThresholdAttach  (p.getAPVTS(), "fetThreshold",  fetThresholdSlider)
    , fetRatioAttach      (p.getAPVTS(), "fetRatio",      fetRatioSlider)
    , fetAttackAttach     (p.getAPVTS(), "fetAttack",     fetAttackSlider)
    , fetReleaseAttach    (p.getAPVTS(), "fetRelease",    fetReleaseSlider)
    , fetMakeupGainAttach (p.getAPVTS(), "fetMakeupGain", fetMakeupGainSlider)
    , fetBypassAttach     (p.getAPVTS(), "fetBypass",     fetBypassButton)
    , optoThresholdAttach  (p.getAPVTS(), "optoThreshold",  optoThresholdSlider)
    , optoRatioAttach      (p.getAPVTS(), "optoRatio",      optoRatioSlider)
    , optoAttackAttach     (p.getAPVTS(), "optoAttack",     optoAttackSlider)
    , optoReleaseAttach    (p.getAPVTS(), "optoRelease",    optoReleaseSlider)
    , optoMakeupGainAttach (p.getAPVTS(), "optoMakeupGain", optoMakeupGainSlider)
    , optoBypassAttach     (p.getAPVTS(), "optoBypass",     optoBypassButton)
{
    for (auto* s : { &fetThresholdSlider, &fetRatioSlider, &fetAttackSlider,
                     &fetReleaseSlider, &fetMakeupGainSlider })
        addAndMakeVisible (s);

    addAndMakeVisible (fetBypassButton);

    for (auto* s : { &optoThresholdSlider, &optoRatioSlider, &optoAttackSlider,
                     &optoReleaseSlider, &optoMakeupGainSlider })
        addAndMakeVisible (s);

    addAndMakeVisible (optoBypassButton);
}

CompressorPanel::~CompressorPanel() {}

void CompressorPanel::paint (juce::Graphics& g)
{
    auto b = getLocalBounds();
    g.fillAll (juce::Colour (0xFF1a1a1a));

    g.setColour (juce::Colours::orange);
    g.setFont (juce::Font (13.0f, juce::Font::bold));
    g.drawText ("FET COMPRESSOR",  b.getX() + 10, b.getY() + 6, b.getWidth() / 2 - 10, 18, juce::Justification::left);
    g.drawText ("OPTO COMPRESSOR", b.getX() + b.getWidth() / 2 + 10, b.getY() + 6, b.getWidth() / 2 - 10, 18, juce::Justification::left);

    g.setColour (juce::Colour (0xFF444444));
    g.drawVerticalLine (b.getCentreX(), 0, (float) b.getHeight());
    g.drawHorizontalLine (b.getBottom() - 1, (float) b.getX(), (float) b.getRight());
}

void CompressorPanel::resized()
{
    auto b = getLocalBounds().reduced (8);
    b.removeFromTop (26);

    int halfW   = b.getWidth() / 2;
    int knobSz  = 70;
    int bypassH = 24;
    int gap     = 6;

    auto layoutSection = [&](juce::Rectangle<int> area,
                              juce::Slider& thresh, juce::Slider& ratio,
                              juce::Slider& attack, juce::Slider& release,
                              juce::Slider& makeup, juce::ToggleButton& bypass)
    {
        area.reduce (4, 0);
        auto row = area.removeFromTop (knobSz);

        auto knobSlot = [&]() { return row.removeFromLeft (knobSz + gap); };

        thresh.setBounds (knobSlot().withHeight (knobSz));
        ratio.setBounds  (knobSlot().withHeight (knobSz));
        attack.setBounds (knobSlot().withHeight (knobSz));
        release.setBounds(knobSlot().withHeight (knobSz));
        makeup.setBounds (knobSlot().withHeight (knobSz));
        bypass.setBounds (area.removeFromTop (bypassH).reduced (4, 0));
    };

    layoutSection (b.removeFromLeft (halfW),
                   fetThresholdSlider, fetRatioSlider, fetAttackSlider,
                   fetReleaseSlider, fetMakeupGainSlider, fetBypassButton);

    layoutSection (b,
                   optoThresholdSlider, optoRatioSlider, optoAttackSlider,
                   optoReleaseSlider, optoMakeupGainSlider, optoBypassButton);
}
