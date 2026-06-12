#include "EQPanel.h"
#include "../PluginProcessor.h"

EQPanel::EQPanel (MC3PluginAudioProcessor& processor) : processor (processor)
{
    lowGainSlider = std::make_unique<juce::Slider> (juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow);
    lowGainSlider->setRange (-12.0, 12.0, 0.1);
    lowGainSlider->setValue (0.0);
    addAndMakeVisible (lowGainSlider.get());

    midGainSlider = std::make_unique<juce::Slider> (juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow);
    midGainSlider->setRange (-12.0, 12.0, 0.1);
    midGainSlider->setValue (0.0);
    addAndMakeVisible (midGainSlider.get());

    highGainSlider = std::make_unique<juce::Slider> (juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow);
    highGainSlider->setRange (-12.0, 12.0, 0.1);
    highGainSlider->setValue (0.0);
    addAndMakeVisible (highGainSlider.get());

    eqBypassButton = std::make_unique<juce::ToggleButton> ("EQ Bypass");
    addAndMakeVisible (eqBypassButton.get());
}

EQPanel::~EQPanel()
{
}

void EQPanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xFF1a1a1a));
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("3-Band EQ", 10, 10, 150, 20, juce::Justification::left);
}

void EQPanel::resized()
{
    auto bounds = getLocalBounds().reduced (10);

    int sliderSize = 60;
    int y = 40;

    lowGainSlider->setBounds (10, y, sliderSize, sliderSize);
    midGainSlider->setBounds (10 + sliderSize + 10, y, sliderSize, sliderSize);
    highGainSlider->setBounds (10 + (sliderSize + 10) * 2, y, sliderSize, sliderSize);
    eqBypassButton->setBounds (10 + (sliderSize + 10) * 3, y + 20, 100, 30);
}
