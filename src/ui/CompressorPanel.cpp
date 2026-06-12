#include "CompressorPanel.h"
#include "MC3LookAndFeel.h"
#include "../PluginProcessor.h"

using namespace mc3::colours;

CompressorPanel::CompressorPanel (MC3PluginAudioProcessor& p) : processor (p)
{
    buildStrip (fet,  "fet",  "FET COMPRESSOR",  amber);
    buildStrip (opto, "opto", "OPTO COMPRESSOR", juce::Colour (0xFFE0A030));
}

CompressorPanel::~CompressorPanel() = default;

void CompressorPanel::buildStrip (Strip& s, const juce::String& prefix,
                                  const juce::String& title, juce::Colour tint)
{
    auto& apvts = processor.getAPVTS();
    s.title = title;
    s.tint  = tint;

    auto mk = [&] (const juce::String& id, const juce::String& cap,
                   const juce::String& suffix, int decimals = 1)
    {
        auto k = std::make_unique<LabeledKnob> (apvts, prefix + id, cap, suffix, decimals);
        addAndMakeVisible (*k);
        return k;
    };

    s.threshold = mk ("Threshold",  "THRESH",  " dB");
    s.ratio     = mk ("Ratio",      "RATIO",   ":1");
    s.attack    = mk ("Attack",     "ATTACK",  " ms");
    s.release   = mk ("Release",    "RELEASE", " ms", 0);
    s.makeup    = mk ("MakeupGain", "MAKEUP",  " dB");
    s.knee      = mk ("KneeWidth",  "KNEE",    " dB");
    s.lookahead = mk ("Lookahead",  "LOOKAHD", " ms");
    s.scFreq    = mk ("SidechainFreq", "SC FREQ", " Hz", 0);

    addAndMakeVisible (s.bypass);
    addAndMakeVisible (s.sidechain);
    s.bypassAttach = std::make_unique<APVTS::ButtonAttachment> (apvts, prefix + "Bypass", s.bypass);
    s.scAttach     = std::make_unique<APVTS::ButtonAttachment> (apvts, prefix + "SidechainEnabled", s.sidechain);
}

void CompressorPanel::paint (juce::Graphics& g)
{
    mc3::drawBrushedMetal (g, getLocalBounds().toFloat(), faceTop, faceBottom);
    paintStrip (g, fet);
    paintStrip (g, opto);

    // divider rail between strips
    g.setColour (juce::Colours::black.withAlpha (0.5f));
    g.drawVerticalLine (getWidth() / 2, 8.0f, (float) getHeight() - 8.0f);
    g.setColour (juce::Colours::white.withAlpha (0.06f));
    g.drawVerticalLine (getWidth() / 2 + 1, 8.0f, (float) getHeight() - 8.0f);
}

void CompressorPanel::paintStrip (juce::Graphics& g, Strip& s)
{
    // engraved title plate
    auto plate = s.titleArea.toFloat();
    mc3::drawBrushedMetal (g, plate, mc3::colours::panelRaised, juce::Colour (0xFF202024), 4.0f);
    g.setColour (s.tint.withAlpha (0.6f));
    g.drawRoundedRectangle (plate.reduced (0.5f), 4.0f, 1.0f);

    // accent lamp
    auto lamp = juce::Rectangle<float> (9.0f, 9.0f).withCentre ({ plate.getX() + 14.0f, plate.getCentreY() });
    g.setColour (s.tint.withAlpha (0.5f)); g.fillEllipse (lamp.expanded (4.0f));
    g.setColour (s.tint);                  g.fillEllipse (lamp);

    mc3::drawEngravedText (g, s.title, s.titleArea.withTrimmedLeft (28),
                           juce::Justification::centredLeft,
                           MC3LookAndFeel::engravedFont (15.0f, true), mc3::colours::textLight);
}

void CompressorPanel::resized()
{
    auto b = getLocalBounds();
    layoutStrip (fet,  b.removeFromLeft (getWidth() / 2));
    layoutStrip (opto, b);
}

void CompressorPanel::layoutStrip (Strip& s, juce::Rectangle<int> area)
{
    s.bounds = area;
    area.reduce (12, 10);

    s.titleArea = area.removeFromTop (26);
    area.removeFromTop (8);

    // toggle row at the bottom
    auto toggles = area.removeFromBottom (28);
    s.bypass.setBounds    (toggles.removeFromLeft (toggles.getWidth() / 2).reduced (4, 2));
    s.sidechain.setBounds (toggles.reduced (4, 2));
    area.removeFromBottom (6);

    // two rows of four knobs
    const int rows = 2, cols = 4;
    const int rowH = area.getHeight() / rows;

    LabeledKnob* grid[2][4] = {
        { s.threshold.get(), s.ratio.get(),     s.attack.get(),    s.release.get() },
        { s.makeup.get(),    s.knee.get(),      s.lookahead.get(), s.scFreq.get()  }
    };

    for (int r = 0; r < rows; ++r)
    {
        auto row = area.removeFromTop (rowH);
        const int colW = row.getWidth() / cols;
        for (int c = 0; c < cols; ++c)
            grid[r][c]->setBounds (row.removeFromLeft (colW).reduced (4));
    }
}
