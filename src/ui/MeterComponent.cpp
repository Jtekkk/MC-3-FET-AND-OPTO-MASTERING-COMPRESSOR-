#include "MeterComponent.h"
#include "MC3LookAndFeel.h"
#include "../dsp/LevelMeter.h"
#include "../dsp/CompressorProcessor.h"

using namespace mc3::colours;

// ════════════════════════════════════════════════════════════════════════════
// VuMeter
// ════════════════════════════════════════════════════════════════════════════
VuMeter::VuMeter (juce::String lbl, float minDb_, float maxDb_, bool reverse_,
                  float redZoneNorm_, std::function<float()> provider_, bool enablePeakHold)
    : label (std::move (lbl)), minDb (minDb_), maxDb (maxDb_), reverse (reverse_),
      redZoneNorm (redZoneNorm_), provider (std::move (provider_)), peakHoldEnabled (enablePeakHold)
{
    currentAngle = targetAngle = peakHoldAngle = angleLeft;
    startTimerHz (30);
}

VuMeter::~VuMeter() { stopTimer(); }

float VuMeter::valueToAngle (float db) const
{
    float norm = juce::jlimit (0.0f, 1.0f, (db - minDb) / (maxDb - minDb));
    if (reverse) norm = 1.0f - norm;
    return angleLeft + norm * (angleRight - angleLeft);
}

void VuMeter::timerCallback()
{
    const float v = provider ? provider() : minDb;
    targetAngle = valueToAngle (v);

    // overload when the needle is pinned in the red zone
    const float redAngle = angleLeft + redZoneNorm * (angleRight - angleLeft);
    overload = reverse ? (targetAngle <= angleLeft + (1.0f - redZoneNorm) * (angleRight - angleLeft) && v <= minDb + (maxDb - minDb) * (1.0f - redZoneNorm))
                       : (targetAngle >= redAngle);

    // ballistics: ease toward target (snappier on attack, slower on release)
    const float coeff = (targetAngle > currentAngle) ? 0.5f : 0.2f;
    currentAngle += (targetAngle - currentAngle) * coeff;

    // peak hold: latch the highest needle position, hold ~1s, then decay
    if (peakHoldEnabled)
    {
        if (currentAngle >= peakHoldAngle)
        {
            peakHoldAngle = currentAngle;
            peakHoldCounter = 30;          // ~1s at 30 Hz
        }
        else if (peakHoldCounter > 0)
        {
            --peakHoldCounter;
        }
        else
        {
            peakHoldAngle += (currentAngle - peakHoldAngle) * 0.08f;
        }
    }

    repaint();
}

void VuMeter::resized()
{
    pivot = { getWidth() * 0.5f, getHeight() * 0.96f };
    needleLen = getHeight() * 0.80f;
    renderFaceplate();
}

void VuMeter::renderFaceplate()
{
    const int w = getWidth(), h = getHeight();
    if (w <= 0 || h <= 0) return;

    faceplate = juce::Image (juce::Image::ARGB, w, h, true);
    juce::Graphics g (faceplate);

    auto bounds = juce::Rectangle<float> (0, 0, (float) w, (float) h);

    // recessed bezel
    mc3::drawBrushedMetal (g, bounds, metalDark, juce::Colour (0xFF2a2a2e), 6.0f);
    auto window = bounds.reduced (7.0f);
    windowBounds = window;
    g.setColour (juce::Colours::black);
    g.fillRoundedRectangle (window, 4.0f);

    // warm amber-lit faceplate
    juce::ColourGradient lit (juce::Colour (0xFF3a2c12), window.getCentreX(), window.getBottom(),
                              juce::Colour (0xFF1a1408), window.getCentreX(), window.getY(), false);
    g.setGradientFill (lit);
    g.fillRoundedRectangle (window, 4.0f);

    // glow from the bottom (backlight)
    juce::ColourGradient glow (amber.withAlpha (0.18f), pivot.x, pivot.y,
                               juce::Colours::transparentBlack, pivot.x, pivot.y - needleLen, true);
    g.setGradientFill (glow);
    g.fillRoundedRectangle (window, 4.0f);

    // scale arc + ticks
    const float scaleR = needleLen * 0.92f;
    const int   numTicks = 11;
    for (int i = 0; i < numTicks; ++i)
    {
        const float t = (float) i / (float) (numTicks - 1);
        const float a = angleLeft + t * (angleRight - angleLeft);
        const bool inRed = (t >= redZoneNorm);
        auto p1 = pivot.getPointOnCircumference (scaleR, a);
        auto p2 = pivot.getPointOnCircumference (scaleR - (i % 5 == 0 ? 9.0f : 5.0f), a);
        g.setColour (inRed ? red.withAlpha (0.9f) : amber.withAlpha (0.8f));
        g.drawLine (p1.x, p1.y, p2.x, p2.y, (i % 5 == 0) ? 1.8f : 1.0f);
    }
    // arc line
    juce::Path arc; arc.addCentredArc (pivot.x, pivot.y, scaleR, scaleR, 0.0f, angleLeft, angleRight, true);
    g.setColour (amber.withAlpha (0.55f));
    g.strokePath (arc, juce::PathStrokeType (1.2f));
    // red segment of arc (only when a red zone is actually on-scale)
    if (redZoneNorm <= 1.0f)
    {
        juce::Path redArc;
        redArc.addCentredArc (pivot.x, pivot.y, scaleR, scaleR, 0.0f,
                              angleLeft + redZoneNorm * (angleRight - angleLeft), angleRight, true);
        g.setColour (red.withAlpha (0.8f));
        g.strokePath (redArc, juce::PathStrokeType (1.6f));
    }

    // scale numbers (5 evenly-spaced marks, like a real VU face)
    for (int k = 0; k <= 4; ++k)
    {
        const float t  = (float) k / 4.0f;
        const float a  = angleLeft + t * (angleRight - angleLeft);
        const float db = minDb + t * (maxDb - minDb);
        auto pos = pivot.getPointOnCircumference (scaleR - 15.0f, a);
        const juce::String txt = (std::abs (db) < 0.5f) ? "0" : juce::String (juce::roundToInt (db));
        g.setFont (MC3LookAndFeel::engravedFont (9.0f, true));
        g.setColour ((t >= redZoneNorm) ? red.withAlpha (0.95f) : amber.withAlpha (0.85f));
        g.drawText (txt, juce::Rectangle<float> (24.0f, 11.0f).withCentre (pos), juce::Justification::centred);
    }

    // "VU" marking
    mc3::drawEngravedText (g, "VU", juce::Rectangle<int> (0, (int) (pivot.y - needleLen * 0.42f), w, 12),
                           juce::Justification::centred, MC3LookAndFeel::engravedFont (8.0f, true),
                           amber.withAlpha (0.6f));

    // label engraved at bottom
    mc3::drawEngravedText (g, label, juce::Rectangle<int> (0, h - 17, w, 14),
                           juce::Justification::centred, MC3LookAndFeel::engravedFont (11.0f, true),
                           amber);
}

void VuMeter::paint (juce::Graphics& g)
{
    if (faceplate.isValid())
        g.drawImageAt (faceplate, 0, 0);

    // peak-hold marker (a short amber tick on the scale arc)
    if (peakHoldEnabled && peakHoldAngle > angleLeft + 0.02f)
    {
        const float scaleR = needleLen * 0.92f;
        auto p1 = pivot.getPointOnCircumference (scaleR + 2.0f, peakHoldAngle);
        auto p2 = pivot.getPointOnCircumference (scaleR - 7.0f, peakHoldAngle);
        const bool inRed = peakHoldAngle >= angleLeft + redZoneNorm * (angleRight - angleLeft);
        g.setColour ((inRed ? red : amber).withAlpha (0.95f));
        g.drawLine (p1.x, p1.y, p2.x, p2.y, 2.2f);
    }

    // needle
    auto tip = pivot.getPointOnCircumference (needleLen, currentAngle);
    g.setColour (juce::Colours::black.withAlpha (0.5f));
    g.drawLine (pivot.x + 1.0f, pivot.y + 1.0f, tip.x + 1.0f, tip.y + 1.0f, 2.6f);
    g.setColour (overload ? red : juce::Colour (0xFFf3e8d0));
    g.drawLine (pivot.x, pivot.y, tip.x, tip.y, 2.0f);

    // hub
    g.setColour (metalLight);
    g.fillEllipse (juce::Rectangle<float> (12.0f, 12.0f).withCentre (pivot));
    g.setColour (juce::Colours::black.withAlpha (0.5f));
    g.drawEllipse (juce::Rectangle<float> (12.0f, 12.0f).withCentre (pivot), 1.0f);

    // overload lamp
    auto lamp = juce::Rectangle<float> (8.0f, 8.0f).withCentre ({ getWidth() - 14.0f, 14.0f });
    if (overload) { g.setColour (red.withAlpha (0.5f)); g.fillEllipse (lamp.expanded (4.0f)); g.setColour (red); }
    else            g.setColour (juce::Colour (0xFF40181a));
    g.fillEllipse (lamp);

    // glass reflection over the window (sits on top of the needle)
    if (! windowBounds.isEmpty())
    {
        juce::Graphics::ScopedSaveState save (g);
        juce::Path clip; clip.addRoundedRectangle (windowBounds, 4.0f);
        g.reduceClipRegion (clip);

        // soft diagonal sheen from the top-left
        juce::ColourGradient sheen (juce::Colours::white.withAlpha (0.10f), windowBounds.getX(), windowBounds.getY(),
                                    juce::Colours::transparentWhite, windowBounds.getCentreX(), windowBounds.getCentreY(), false);
        g.setGradientFill (sheen);
        g.fillRect (windowBounds);

        // curved glare highlight across the upper portion
        juce::Path glare;
        glare.addEllipse (windowBounds.getX() - windowBounds.getWidth() * 0.25f,
                          windowBounds.getY() - windowBounds.getHeight() * 0.75f,
                          windowBounds.getWidth() * 1.1f, windowBounds.getHeight() * 1.0f);
        g.setColour (juce::Colours::white.withAlpha (0.06f));
        g.fillPath (glare);
    }
}

// ════════════════════════════════════════════════════════════════════════════
// MeterPanel
// ════════════════════════════════════════════════════════════════════════════
MeterPanel::MeterPanel (const LevelMeter& in, const LevelMeter& out,
                        const CompressorProcessor& fet, const CompressorProcessor& opto)
{
    inputMeter  = std::make_unique<VuMeter> ("INPUT",  -40.0f, 0.0f, false, 0.85f,
                                             [&in]  { return in.getPeakLevelDb(); }, true);
    outputMeter = std::make_unique<VuMeter> ("OUTPUT", -40.0f, 0.0f, false, 0.85f,
                                             [&out] { return out.getPeakLevelDb(); }, true);
    // GR meters: rest at the right (0 dB reduction), swing left as they compress;
    // no red zone (reverse=false, red disabled off-scale).
    fetGr  = std::make_unique<VuMeter> ("FET  GR",  -24.0f, 0.0f, false, 2.0f,
                                        [&fet]  { return fet.getGainReductionDb(); });
    optoGr = std::make_unique<VuMeter> ("OPTO  GR", -24.0f, 0.0f, false, 2.0f,
                                        [&opto] { return opto.getGainReductionDb(); });

    addAndMakeVisible (*inputMeter);
    addAndMakeVisible (*fetGr);
    addAndMakeVisible (*optoGr);
    addAndMakeVisible (*outputMeter);
}

MeterPanel::~MeterPanel() = default;

void MeterPanel::paint (juce::Graphics& g)
{
    mc3::drawBrushedMetal (g, getLocalBounds().toFloat(), juce::Colour (0xFF26262b), juce::Colour (0xFF17171b));
    g.setColour (juce::Colours::black.withAlpha (0.5f));
    g.drawHorizontalLine (getHeight() - 1, 0.0f, (float) getWidth());
}

void MeterPanel::resized()
{
    auto b = getLocalBounds().reduced (10, 8);
    const int n = 4;
    const int gap = 10;
    const int w = (b.getWidth() - gap * (n - 1)) / n;

    inputMeter->setBounds  (b.removeFromLeft (w)); b.removeFromLeft (gap);
    fetGr->setBounds       (b.removeFromLeft (w)); b.removeFromLeft (gap);
    optoGr->setBounds      (b.removeFromLeft (w)); b.removeFromLeft (gap);
    outputMeter->setBounds (b.removeFromLeft (w));
}
