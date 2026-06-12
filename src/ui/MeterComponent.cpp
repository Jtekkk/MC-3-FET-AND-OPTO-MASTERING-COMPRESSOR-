#include "MeterComponent.h"
#include "../dsp/LevelMeter.h"
#include "../utils/Utilities.h"

// ────────────────────────────────────────────────────────────────────────────
// MeterDisplay
// ────────────────────────────────────────────────────────────────────────────

MeterDisplay::MeterDisplay (const LevelMeter& m, const juce::String& lbl)
    : meter (m), label (lbl)
{
    startTimer (50);  // Update every 50ms
}

MeterDisplay::~MeterDisplay()
{
    stopTimer();
}

void MeterDisplay::timerCallback()
{
    const float newPeak = meter.getPeakLevelDb();
    const bool newClipping = meter.isClipping();

    if (newPeak != displayPeakDb || newClipping != displayClipping)
    {
        displayPeakDb = newPeak;
        displayClipping = newClipping;
        repaint();
    }
}

void MeterDisplay::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    // Background
    g.fillAll (juce::Colour (0xFF0a0a0a));
    g.setColour (juce::Colour (0xFF1a1a1a));
    g.fillRect (bounds.reduced (1));

    // Draw scale and segments
    g.setColour (juce::Colour (0xFF333333));
    g.setFont (10.0f);

    const int labelW = 30;
    const int meterArea = bounds.getWidth() - labelW;

    for (int db = METER_MIN_DB; db <= METER_MAX_DB; db += 6)
    {
        const float norm = (float) (db - METER_MIN_DB) / (float) (METER_MAX_DB - METER_MIN_DB);
        const int x = labelW + (int) (norm * meterArea);

        g.drawVerticalLine (x, (float) bounds.getY(), (float) bounds.getBottom());
        g.drawText (juce::String (db), x - 12, bounds.getBottom() - 12, 24, 10,
                    juce::Justification::centred);
    }

    // Draw meter bar
    const float normPeak = (displayPeakDb - METER_MIN_DB) / (float) (METER_MAX_DB - METER_MIN_DB);
    const float barWidth = juce::jlimit (0.0f, 1.0f, normPeak) * meterArea;

    if (barWidth > 0.0f)
    {
        // Color gradient: green → yellow → red
        juce::Colour meterColour;
        if (displayPeakDb < -12.0f)
            meterColour = juce::Colours::limegreen;
        else if (displayPeakDb < -6.0f)
            meterColour = juce::Colour (0xFFffff00);  // Yellow
        else
            meterColour = juce::Colours::red;

        if (displayClipping)
            meterColour = juce::Colours::darkred;

        g.setColour (meterColour);
        g.fillRect (labelW, 6, (int) barWidth, bounds.getHeight() - 20);
    }

    // Draw border and label
    g.setColour (juce::Colours::white.withAlpha (0.3f));
    g.drawRect (labelW, 6, meterArea, bounds.getHeight() - 20, 1);

    g.setColour (juce::Colours::white);
    g.setFont (juce::Font (11.0f, juce::Font::bold));
    g.drawText (label, 2, bounds.getY() + 6, labelW - 4, 14, juce::Justification::right);

    // Clipping indicator
    if (displayClipping)
    {
        g.setColour (juce::Colours::red);
        g.setFont (juce::Font (9.0f, juce::Font::bold));
        g.drawText ("CLIP", 2, bounds.getBottom() - 14, labelW - 4, 12,
                    juce::Justification::centred);
    }

    // Current level text
    g.setColour (juce::Colours::white.withAlpha (0.6f));
    g.setFont (10.0f);
    g.drawText (juce::String (displayPeakDb, 1) + " dB",
                bounds.getRight() - 50, bounds.getY() + 6, 48, 14,
                juce::Justification::right);
}

void MeterDisplay::resized()
{
    // Just paint, no child components
}

// ────────────────────────────────────────────────────────────────────────────
// MeterPanel
// ────────────────────────────────────────────────────────────────────────────

MeterPanel::MeterPanel (const LevelMeter& inputMeter, const LevelMeter& outputMeter)
{
    inputDisplay = std::make_unique<MeterDisplay> (inputMeter, "IN");
    outputDisplay = std::make_unique<MeterDisplay> (outputMeter, "OUT");

    addAndMakeVisible (inputDisplay.get());
    addAndMakeVisible (outputDisplay.get());
}

MeterPanel::~MeterPanel() {}

void MeterPanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xFF222222));
    g.setColour (juce::Colour (0xFF444444));
    g.drawHorizontalLine (getHeight() - 1, 0.0f, (float) getWidth());
}

void MeterPanel::resized()
{
    auto b = getLocalBounds().reduced (8, 4);

    inputDisplay->setBounds (b.removeFromTop (b.getHeight() / 2).reduced (0, 2));
    outputDisplay->setBounds (b.reduced (0, 2));
}
