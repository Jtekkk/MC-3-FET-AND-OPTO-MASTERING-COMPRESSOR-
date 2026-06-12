#include "CompressorPanel.h"
#include "../PluginProcessor.h"

CompressorPanel::CompressorPanel (MC3PluginAudioProcessor& processor) : processor (processor)
{
    // FET Compressor
    fetThresholdSlider = std::make_unique<juce::Slider> (juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow);
    fetThresholdSlider->setRange (-60.0, 0.0, 0.1);
    fetThresholdSlider->setValue (-20.0);
    addAndMakeVisible (fetThresholdSlider.get());

    fetRatioSlider = std::make_unique<juce::Slider> (juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow);
    fetRatioSlider->setRange (1.0, 20.0, 0.1);
    fetRatioSlider->setValue (4.0);
    addAndMakeVisible (fetRatioSlider.get());

    fetAttackSlider = std::make_unique<juce::Slider> (juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow);
    fetAttackSlider->setRange (0.1, 100.0, 0.1);
    fetAttackSlider->setValue (10.0);
    addAndMakeVisible (fetAttackSlider.get());

    fetReleaseSlider = std::make_unique<juce::Slider> (juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow);
    fetReleaseSlider->setRange (10.0, 2000.0, 1.0);
    fetReleaseSlider->setValue (100.0);
    addAndMakeVisible (fetReleaseSlider.get());

    fetMakeupGainSlider = std::make_unique<juce::Slider> (juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow);
    fetMakeupGainSlider->setRange (0.0, 60.0, 0.1);
    fetMakeupGainSlider->setValue (0.0);
    addAndMakeVisible (fetMakeupGainSlider.get());

    fetBypassButton = std::make_unique<juce::ToggleButton> ("FET Bypass");
    addAndMakeVisible (fetBypassButton.get());

    // Opto Compressor
    optoThresholdSlider = std::make_unique<juce::Slider> (juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow);
    optoThresholdSlider->setRange (-60.0, 0.0, 0.1);
    optoThresholdSlider->setValue (-20.0);
    addAndMakeVisible (optoThresholdSlider.get());

    optoRatioSlider = std::make_unique<juce::Slider> (juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow);
    optoRatioSlider->setRange (1.0, 20.0, 0.1);
    optoRatioSlider->setValue (4.0);
    addAndMakeVisible (optoRatioSlider.get());

    optoAttackSlider = std::make_unique<juce::Slider> (juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow);
    optoAttackSlider->setRange (0.1, 100.0, 0.1);
    optoAttackSlider->setValue (10.0);
    addAndMakeVisible (optoAttackSlider.get());

    optoReleaseSlider = std::make_unique<juce::Slider> (juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow);
    optoReleaseSlider->setRange (10.0, 2000.0, 1.0);
    optoReleaseSlider->setValue (100.0);
    addAndMakeVisible (optoReleaseSlider.get());

    optoMakeupGainSlider = std::make_unique<juce::Slider> (juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow);
    optoMakeupGainSlider->setRange (0.0, 60.0, 0.1);
    optoMakeupGainSlider->setValue (0.0);
    addAndMakeVisible (optoMakeupGainSlider.get());

    optoBypassButton = std::make_unique<juce::ToggleButton> ("Opto Bypass");
    addAndMakeVisible (optoBypassButton.get());
}

CompressorPanel::~CompressorPanel()
{
}

void CompressorPanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xFF1a1a1a));
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("FET Compressor", 10, 10, 150, 20, juce::Justification::left);
}

void CompressorPanel::resized()
{
    auto bounds = getLocalBounds().reduced (10);
    auto width = bounds.getWidth();
    auto height = bounds.getHeight();

    int sliderSize = 60;
    int y = 40;

    fetThresholdSlider->setBounds (10, y, sliderSize, sliderSize);
    fetRatioSlider->setBounds (10 + sliderSize + 10, y, sliderSize, sliderSize);
    fetAttackSlider->setBounds (10 + (sliderSize + 10) * 2, y, sliderSize, sliderSize);
    fetReleaseSlider->setBounds (10 + (sliderSize + 10) * 3, y, sliderSize, sliderSize);
    fetMakeupGainSlider->setBounds (10 + (sliderSize + 10) * 4, y, sliderSize, sliderSize);
    fetBypassButton->setBounds (10 + (sliderSize + 10) * 5, y + 20, 100, 30);
}
