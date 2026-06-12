#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace mc3
{
    // ── Vintage hardware palette (amber accent) ───────────────────────────────
    namespace colours
    {
        const juce::Colour faceTop      { 0xFF3c3c42 };  // brushed faceplate top
        const juce::Colour faceBottom   { 0xFF1f1f24 };  // brushed faceplate bottom
        const juce::Colour panelInset   { 0xFF141417 };  // recessed wells
        const juce::Colour panelRaised  { 0xFF2c2c31 };  // raised sub-panels
        const juce::Colour metalLight   { 0xFFc4c4cb };  // knob cap highlight
        const juce::Colour metalMid     { 0xFF7d7d85 };
        const juce::Colour metalDark    { 0xFF45454b };  // knob skirt
        const juce::Colour amber        { 0xFFFFA61E };  // primary accent
        const juce::Colour amberBright  { 0xFFFFD27A };  // glowing highlight
        const juce::Colour amberDeep    { 0xFFC2660A };
        const juce::Colour engrave      { 0xFF0d0d10 };  // engraved text shadow
        const juce::Colour engraveHi    { 0x33FFFFFF };  // emboss highlight
        const juce::Colour textLight    { 0xFFd2d2d8 };
        const juce::Colour textDim      { 0xFF8a8a90 };
        const juce::Colour red          { 0xFFE2433A };
        const juce::Colour green        { 0xFF6FCF6A };
    }

    // ── Shared drawing helpers ────────────────────────────────────────────────
    void drawBrushedMetal (juce::Graphics&, juce::Rectangle<float> area,
                           juce::Colour top, juce::Colour bottom, float cornerRadius = 0.0f);

    void drawScrew (juce::Graphics&, juce::Point<float> centre, float radius);

    void drawRecessedWell (juce::Graphics&, juce::Rectangle<float> area, float cornerRadius);

    void drawEngravedText (juce::Graphics&, const juce::String& text, juce::Rectangle<int> area,
                           juce::Justification, const juce::Font&,
                           juce::Colour textColour = colours::textLight);
}

class MC3LookAndFeel : public juce::LookAndFeel_V4
{
public:
    MC3LookAndFeel();
    ~MC3LookAndFeel() override = default;

    void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPosProportional, float rotaryStartAngle,
                           float rotaryEndAngle, juce::Slider&) override;

    void drawLinearSlider (juce::Graphics&, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           juce::Slider::SliderStyle, juce::Slider&) override;

    void drawToggleButton (juce::Graphics&, juce::ToggleButton&,
                           bool shouldDrawButtonAsHighlighted,
                           bool shouldDrawButtonAsDown) override;

    void drawButtonBackground (juce::Graphics&, juce::Button&, const juce::Colour& backgroundColour,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown) override;

    void drawComboBox (juce::Graphics&, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       juce::ComboBox&) override;

    void positionComboBoxText (juce::ComboBox&, juce::Label&) override;

    juce::Font getComboBoxFont (juce::ComboBox&) override;
    juce::Font getLabelFont (juce::Label&) override;
    juce::Font getTextButtonFont (juce::TextButton&, int buttonHeight) override;

    juce::Label* createSliderTextBox (juce::Slider&) override;

    static juce::Font engravedFont (float height, bool bold = true);
};
