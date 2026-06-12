#include "MeterComponent.h"
#include "../dsp/LevelMeter.h"
#include "../dsp/CompressorProcessor.h"
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
    g.setFont (9.0f);

    const int labelW = 32;
    const int meterArea = bounds.getWidth() - labelW;

    for (int db = METER_MIN_DB; db <= METER_MAX_DB; db += 6)
    {
        const float norm = (float) (db - METER_MIN_DB) / (float) (METER_MAX_DB - METER_MIN_DB);
        const int x = labelW + (int) (norm * meterArea);

        g.drawVerticalLine (x, (float) bounds.getY(), (float) bounds.getBottom());
        g.drawText (juce::String (db), x - 12, bounds.getBottom() - 11, 24, 9,
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
        g.fillRect (labelW, 3, (int) barWidth, bounds.getHeight() - 13);
    }

    // Draw border and label
    g.setColour (juce::Colours::white.withAlpha (0.3f));
    g.drawRect (labelW, 3, meterArea, bounds.getHeight() - 13, 1);

    g.setColour (juce::Colours::white);
    g.setFont (juce::Font (10.0f, juce::Font::bold));
    g.drawText (label, 2, bounds.getY() + 2, labelW - 4, 11, juce::Justification::right);

    // Clipping indicator
    if (displayClipping)
    {
        g.setColour (juce::Colours::red);
        g.setFont (juce::Font (8.0f, juce::Font::bold));
        g.drawText ("CLIP", 2, bounds.getBottom() - 10, labelW - 4, 9,
                    juce::Justification::centred);
    }

    // Current level text
    g.setColour (juce::Colours::white.withAlpha (0.6f));
    g.setFont (9.0f);
    g.drawText (juce::String (displayPeakDb, 1) + "dB",
                bounds.getRight() - 45, bounds.getY() + 2, 43, 11,
                juce::Justification::right);
}

void MeterDisplay::resized()
{
    // Just paint, no child components
}

// ────────────────────────────────────────────────────────────────────────────
// GainReductionMeter
// ────────────────────────────────────────────────────────────────────────────

GainReductionMeter::GainReductionMeter (const CompressorProcessor& comp, const juce::String& lbl)
    : compressor (comp), label (lbl)
{
    startTimer (50);
}

GainReductionMeter::~GainReductionMeter()
{
    stopTimer();
}

void GainReductionMeter::timerCallback()
{
    const float newGr = compressor.getGainReductionDb();
    if (newGr != displayGrDb)
    {
        displayGrDb = newGr;
        repaint();
    }
}

void GainReductionMeter::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds();

    // Background
    g.fillAll (juce::Colour (0xFF0a0a0a));
    g.setColour (juce::Colour (0xFF1a1a1a));
    g.fillRect (bounds.reduced (1));

    // Draw scale
    g.setColour (juce::Colour (0xFF333333));
    g.setFont (8.0f);

    const int labelW = 32;
    const int meterArea = bounds.getWidth() - labelW;

    for (int db = GR_MIN_DB; db <= GR_MAX_DB; db += 6)
    {
        const float norm = (float) (db - GR_MIN_DB) / (float) (GR_MAX_DB - GR_MIN_DB);
        const int x = labelW + (int) (norm * meterArea);
        g.drawVerticalLine (x, (float) bounds.getY(), (float) bounds.getBottom());
    }

    // Draw GR bar (shows reduction, so starts from right at 0dB, goes left toward -30dB)
    const float grNorm = (displayGrDb - GR_MIN_DB) / (float) (GR_MAX_DB - GR_MIN_DB);
    const float barWidth = juce::jlimit (0.0f, 1.0f, -grNorm) * meterArea;  // Negative GR

    if (barWidth > 0.1f)
    {
        g.setColour (juce::Colour (0xFF0099ff));  // Cyan for GR
        g.fillRect (labelW, 3, (int) barWidth, bounds.getHeight() - 13);
    }

    // Draw border
    g.setColour (juce::Colours::white.withAlpha (0.3f));
    g.drawRect (labelW, 3, meterArea, bounds.getHeight() - 13, 1);

    // Label
    g.setColour (juce::Colours::white);
    g.setFont (juce::Font (9.0f, juce::Font::bold));
    g.drawText (label, 2, bounds.getY() + 2, labelW - 4, 11, juce::Justification::right);

    // GR value
    g.setColour (juce::Colours::cyan);
    g.setFont (8.0f);
    g.drawText (juce::String (displayGrDb, 1) + "dB",
                bounds.getRight() - 45, bounds.getY() + 2, 43, 11,
                juce::Justification::right);
}

void GainReductionMeter::resized() {}

// ────────────────────────────────────────────────────────────────────────────
// MeterPanel
// ────────────────────────────────────────────────────────────────────────────

MeterPanel::MeterPanel (const LevelMeter& inputMeter, const LevelMeter& outputMeter,
                        const CompressorProcessor& fetComp, const CompressorProcessor& optoComp)
{
    inputDisplay = std::make_unique<MeterDisplay> (inputMeter, "IN");
    outputDisplay = std::make_unique<MeterDisplay> (outputMeter, "OUT");
    fetGrMeter = std::make_unique<GainReductionMeter> (fetComp, "FET");
    optoGrMeter = std::make_unique<GainReductionMeter> (optoComp, "OPTO");

    addAndMakeVisible (inputDisplay.get());
    addAndMakeVisible (outputDisplay.get());
    addAndMakeVisible (fetGrMeter.get());
    addAndMakeVisible (optoGrMeter.get());
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
    auto b = getLocalBounds().reduced (8, 3);

    const int rowHeight = b.getHeight() / 2;

    // Top row: input level, FET GR
    auto topRow = b.removeFromTop (rowHeight).reduced (0, 1);
    inputDisplay->setBounds (topRow.removeFromLeft (b.getWidth() / 2));
    fetGrMeter->setBounds (topRow);

    // Bottom row: output level, Opto GR
    auto bottomRow = b.reduced (0, 1);
    outputDisplay->setBounds (bottomRow.removeFromLeft (b.getWidth() / 2));
    optoGrMeter->setBounds (bottomRow);
}
