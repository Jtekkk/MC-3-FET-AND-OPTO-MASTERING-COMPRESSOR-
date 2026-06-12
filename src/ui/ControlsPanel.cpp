#include "ControlsPanel.h"
#include "../PluginProcessor.h"

ControlsPanel::ControlsPanel (MC3PluginAudioProcessor& processor) : processor (processor)
{
    inputTransformerBox = std::make_unique<juce::ComboBox>();
    inputTransformerBox->addItem ("Bright", 1);
    inputTransformerBox->addItem ("Neutral", 2);
    inputTransformerBox->addItem ("Warm", 3);
    inputTransformerBox->setSelectedId (2);
    addAndMakeVisible (inputTransformerBox.get());

    outputTransformerBox = std::make_unique<juce::ComboBox>();
    outputTransformerBox->addItem ("Bright", 1);
    outputTransformerBox->addItem ("Neutral", 2);
    outputTransformerBox->addItem ("Warm", 3);
    outputTransformerBox->setSelectedId (2);
    addAndMakeVisible (outputTransformerBox.get());

    oversamplingButton = std::make_unique<juce::ToggleButton> ("8x Oversampling");
    oversamplingButton->setToggleState (true, juce::NotificationType::dontSendNotification);
    addAndMakeVisible (oversamplingButton.get());

    outputGainSlider = std::make_unique<juce::Slider> (juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight);
    outputGainSlider->setRange (-12.0, 12.0, 0.1);
    outputGainSlider->setValue (0.0);
    addAndMakeVisible (outputGainSlider.get());
}

ControlsPanel::~ControlsPanel()
{
}

void ControlsPanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xFF1a1a1a));
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("Controls", 10, 10, 150, 20, juce::Justification::left);
    g.drawText ("Input Transformer:", 10, 50, 150, 20, juce::Justification::left);
    g.drawText ("Output Transformer:", 10, 100, 150, 20, juce::Justification::left);
    g.drawText ("Output Gain:", 10, 200, 150, 20, juce::Justification::left);
}

void ControlsPanel::resized()
{
    inputTransformerBox->setBounds (160, 50, 150, 25);
    outputTransformerBox->setBounds (160, 100, 150, 25);
    oversamplingButton->setBounds (160, 150, 150, 30);
    outputGainSlider->setBounds (160, 200, 400, 30);
}
