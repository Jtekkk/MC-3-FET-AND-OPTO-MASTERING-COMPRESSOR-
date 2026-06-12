#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "MC3LookAndFeel.h"

// A rotary knob with an engraved caption underneath, bound to an APVTS parameter.
class LabeledKnob : public juce::Component
{
public:
    LabeledKnob (juce::AudioProcessorValueTreeState& apvts,
                 const juce::String& paramID, const juce::String& caption,
                 const juce::String& suffix = {}, int decimals = 1)
        : labelText (caption)
    {
        slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setRotaryParameters (juce::MathConstants<float>::pi * 1.2f,
                                    juce::MathConstants<float>::pi * 2.8f, true);
        slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 70, 16);
        slider.setColour (juce::Slider::textBoxTextColourId, mc3::colours::amberBright);
        addAndMakeVisible (slider);

        // Attachment sets the range/value (and resets decimals), so install an
        // explicit formatter AFTER it is constructed — this overrides the slider's
        // auto-computed decimal places for continuous parameter ranges.
        attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
            apvts, paramID, slider);

        slider.textFromValueFunction = [decimals, suffix] (double v)
        {
            return juce::String (v, decimals) + suffix;
        };
        slider.valueFromTextFunction = [] (const juce::String& t)
        {
            return t.retainCharacters ("0123456789.-+eE").getDoubleValue();
        };
        slider.updateText();
    }

    void resized() override
    {
        auto b = getLocalBounds();
        captionArea = b.removeFromBottom (15);
        slider.setBounds (b);
    }

    void paint (juce::Graphics& g) override
    {
        mc3::drawEngravedText (g, labelText.toUpperCase(), captionArea,
                               juce::Justification::centred,
                               MC3LookAndFeel::engravedFont (11.0f, true),
                               mc3::colours::textLight);
    }

    juce::Slider slider;

private:
    juce::String labelText;
    juce::Rectangle<int> captionArea;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabeledKnob)
};
