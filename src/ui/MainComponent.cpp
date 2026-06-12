#include "MainComponent.h"
#include "PresetPanel.h"
#include "CompressorPanel.h"
#include "EQPanel.h"
#include "ControlsPanel.h"
#include "MeterComponent.h"
#include "../PluginProcessor.h"

using namespace mc3::colours;

MainComponent::MainComponent (MC3PluginAudioProcessor& p) : processor (p)
{
    setLookAndFeel (&lookAndFeel);

    presetPanel     = std::make_unique<PresetPanel> (processor.getPresetManager());
    meterPanel      = std::make_unique<MeterPanel> (processor.getInputMeter(), processor.getOutputMeter(),
                                                    processor.getFetCompressor(), processor.getOptoCompressor());
    compressorPanel = std::make_unique<CompressorPanel> (processor);
    eqPanel         = std::make_unique<EQPanel> (processor);
    controlsPanel   = std::make_unique<ControlsPanel> (processor);

    addAndMakeVisible (*presetPanel);
    addAndMakeVisible (*meterPanel);
    addAndMakeVisible (*compressorPanel);
    addAndMakeVisible (*eqPanel);
    addAndMakeVisible (*controlsPanel);
}

MainComponent::~MainComponent()
{
    setLookAndFeel (nullptr);
}

void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (faceBottom);
    paintHeader (g, headerArea);

    // outer faceplate screws
    const float r = 7.0f, m = 16.0f;
    mc3::drawScrew (g, { m, m }, r);
    mc3::drawScrew (g, { getWidth() - m, m }, r);
    mc3::drawScrew (g, { m, getHeight() - m }, r);
    mc3::drawScrew (g, { getWidth() - m, getHeight() - m }, r);
}

void MainComponent::paintOverChildren (juce::Graphics& g)
{
    // Subtle vignette across the whole faceplate for depth.
    auto b = getLocalBounds().toFloat();
    juce::ColourGradient vig (juce::Colours::transparentBlack, b.getCentre(),
                              juce::Colours::black.withAlpha (0.30f), b.getTopLeft(), true);
    vig.addColour (0.65, juce::Colours::transparentBlack);
    g.setGradientFill (vig);
    g.fillRect (b);

    // crisp inner border bevel
    g.setColour (juce::Colours::black.withAlpha (0.6f));
    g.drawRect (b, 1.5f);
    g.setColour (juce::Colours::white.withAlpha (0.05f));
    g.drawRect (b.reduced (1.5f), 1.0f);
}

void MainComponent::paintHeader (juce::Graphics& g, juce::Rectangle<int> area)
{
    mc3::drawBrushedMetal (g, area.toFloat(), juce::Colour (0xFF44444a), juce::Colour (0xFF2a2a30));
    g.setColour (juce::Colours::black.withAlpha (0.5f));
    g.drawHorizontalLine (area.getBottom() - 1, 0.0f, (float) getWidth());
    g.setColour (amber.withAlpha (0.35f));
    g.drawHorizontalLine (area.getBottom() - 2, 0.0f, (float) getWidth());

    // logo block
    auto logo = area.withTrimmedLeft (40).withWidth (260);

    // "MC-3" big engraved with amber face
    auto bigArea = logo.removeFromTop (44).withTrimmedTop (8);
    mc3::drawEngravedText (g, "MC-3", bigArea, juce::Justification::centredLeft,
                           MC3LookAndFeel::engravedFont (34.0f, true), amber);

    mc3::drawEngravedText (g, "FET / OPTO  MASTERING  COMPRESSOR",
                           logo, juce::Justification::centredLeft,
                           MC3LookAndFeel::engravedFont (10.5f, true), textDim);

    // power LED + label, centre-left of header
    auto led = juce::Rectangle<float> (12.0f, 12.0f).withCentre ({ 320.0f, area.getCentreY() - 4.0f });
    g.setColour (amber.withAlpha (0.5f)); g.fillEllipse (led.expanded (5.0f));
    g.setColour (amberBright);            g.fillEllipse (led);
    g.setColour (juce::Colours::black.withAlpha (0.4f)); g.drawEllipse (led, 1.0f);
    mc3::drawEngravedText (g, "POWER", juce::Rectangle<int> (300, (int) area.getCentreY() + 6, 44, 12),
                           juce::Justification::centred, MC3LookAndFeel::engravedFont (8.0f, true), textDim);
}

void MainComponent::resized()
{
    auto b = getLocalBounds();

    headerArea = b.removeFromTop (76);
    // preset panel docked to the right of the header
    presetPanel->setBounds (headerArea.withTrimmedRight (40).withTrimmedLeft (getWidth() - 380)
                                       .withSizeKeepingCentre (320, 44));

    meterPanel->setBounds (b.removeFromTop (168));

    auto bottom = b.removeFromBottom (188);
    compressorPanel->setBounds (b);

    // bottom split: EQ (left) + output controls (right)
    eqPanel->setBounds       (bottom.removeFromLeft (juce::roundToInt (getWidth() * 0.42f)));
    controlsPanel->setBounds (bottom);
}
