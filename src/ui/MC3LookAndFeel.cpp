#include "MC3LookAndFeel.h"

using namespace mc3::colours;

// ════════════════════════════════════════════════════════════════════════════
// Drawing helpers
// ════════════════════════════════════════════════════════════════════════════
namespace mc3
{
juce::Font MC3LookAndFeelFontHelper (float, bool); // fwd (unused placeholder)

void drawBrushedMetal (juce::Graphics& g, juce::Rectangle<float> area,
                       juce::Colour top, juce::Colour bottom, float cornerRadius)
{
    // Base vertical gradient
    juce::ColourGradient grad (top, area.getX(), area.getY(),
                               bottom, area.getX(), area.getBottom(), false);
    g.setGradientFill (grad);
    if (cornerRadius > 0.0f) g.fillRoundedRectangle (area, cornerRadius);
    else                     g.fillRect (area);

    // Fine horizontal brushed striations
    juce::Graphics::ScopedSaveState save (g);
    if (cornerRadius > 0.0f)
    {
        juce::Path clip; clip.addRoundedRectangle (area, cornerRadius);
        g.reduceClipRegion (clip);
    }
    else
    {
        g.reduceClipRegion (area.toNearestInt());
    }

    juce::Random rng (0x4d433321 ^ (int) area.getWidth() ^ ((int) area.getHeight() << 8));
    const int y0 = (int) area.getY();
    const int y1 = (int) area.getBottom();
    for (int y = y0; y < y1; ++y)
    {
        const float a = (rng.nextFloat() - 0.5f) * 0.06f;
        g.setColour ((a >= 0.0f ? juce::Colours::white : juce::Colours::black).withAlpha (std::abs (a)));
        g.drawHorizontalLine (y, area.getX(), area.getRight());
    }

    // Subtle top sheen
    juce::ColourGradient sheen (juce::Colours::white.withAlpha (0.08f), area.getX(), area.getY(),
                                juce::Colours::transparentWhite, area.getX(), area.getY() + area.getHeight() * 0.4f, false);
    g.setGradientFill (sheen);
    if (cornerRadius > 0.0f) g.fillRoundedRectangle (area, cornerRadius);
    else                     g.fillRect (area);
}

void drawScrew (juce::Graphics& g, juce::Point<float> centre, float radius)
{
    auto b = juce::Rectangle<float> (radius * 2.0f, radius * 2.0f).withCentre (centre);

    // recessed hole
    g.setColour (juce::Colours::black.withAlpha (0.5f));
    g.fillEllipse (b.expanded (1.5f));

    // metal head
    juce::ColourGradient cg (metalLight, b.getX(), b.getY(), metalDark, b.getRight(), b.getBottom(), false);
    g.setGradientFill (cg);
    g.fillEllipse (b);
    g.setColour (juce::Colours::black.withAlpha (0.4f));
    g.drawEllipse (b, 1.0f);

    // slot (slightly rotated per position for realism)
    const float angle = juce::MathConstants<float>::pi * (0.15f + 0.5f * std::sin (centre.x + centre.y));
    juce::Point<float> d (std::cos (angle) * radius * 0.7f, std::sin (angle) * radius * 0.7f);
    g.setColour (juce::Colours::black.withAlpha (0.6f));
    g.drawLine (centre.x - d.x, centre.y - d.y, centre.x + d.x, centre.y + d.y, 1.6f);
    g.setColour (juce::Colours::white.withAlpha (0.15f));
    g.drawLine (centre.x - d.x, centre.y - d.y + 1.0f, centre.x + d.x, centre.y + d.y + 1.0f, 1.0f);
}

void drawRecessedWell (juce::Graphics& g, juce::Rectangle<float> area, float cornerRadius)
{
    g.setColour (panelInset);
    g.fillRoundedRectangle (area, cornerRadius);
    // inner shadow top, light bottom for inset illusion
    g.setColour (juce::Colours::black.withAlpha (0.55f));
    g.drawRoundedRectangle (area.reduced (0.5f), cornerRadius, 1.4f);
    g.setColour (juce::Colours::white.withAlpha (0.06f));
    g.drawLine (area.getX() + cornerRadius, area.getBottom() - 1.0f,
                area.getRight() - cornerRadius, area.getBottom() - 1.0f, 1.0f);
}

void drawEngravedText (juce::Graphics& g, const juce::String& text, juce::Rectangle<int> area,
                       juce::Justification just, const juce::Font& font, juce::Colour textColour)
{
    g.setFont (font);
    // dark engraved shadow
    g.setColour (engrave.withAlpha (0.9f));
    g.drawText (text, area.translated (0, 1), just, false);
    // emboss highlight
    g.setColour (engraveHi);
    g.drawText (text, area.translated (0, -1), just, false);
    // face
    g.setColour (textColour);
    g.drawText (text, area, just, false);
}
} // namespace mc3

// ════════════════════════════════════════════════════════════════════════════
// MC3LookAndFeel
// ════════════════════════════════════════════════════════════════════════════
MC3LookAndFeel::MC3LookAndFeel()
{
    setColour (juce::Slider::textBoxTextColourId,     amberBright);
    setColour (juce::Slider::textBoxOutlineColourId,  juce::Colours::transparentBlack);
    setColour (juce::Slider::textBoxBackgroundColourId, juce::Colours::transparentBlack);

    setColour (juce::Label::textColourId, textLight);

    setColour (juce::ComboBox::backgroundColourId, panelInset);
    setColour (juce::ComboBox::textColourId,       amberBright);
    setColour (juce::ComboBox::outlineColourId,    juce::Colours::black);
    setColour (juce::ComboBox::arrowColourId,      amber);

    setColour (juce::PopupMenu::backgroundColourId,        juce::Colour (0xFF202024));
    setColour (juce::PopupMenu::textColourId,              textLight);
    setColour (juce::PopupMenu::highlightedBackgroundColourId, amberDeep);
    setColour (juce::PopupMenu::highlightedTextColourId,   juce::Colours::white);

    setColour (juce::TextButton::buttonColourId,   panelRaised);
    setColour (juce::TextButton::textColourOnId,   juce::Colours::black);
    setColour (juce::TextButton::textColourOffId,  textLight);
}

juce::Font MC3LookAndFeel::engravedFont (float height, bool bold)
{
    return juce::Font (height, bold ? juce::Font::bold : juce::Font::plain);
}

// ── Rotary knob: chunky brushed-metal cap, tick ring, amber pointer + arc ─────
void MC3LookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                       float pos, float startAngle, float endAngle, juce::Slider&)
{
    auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (6.0f);
    auto centre = bounds.getCentre();
    const float radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
    const float angle  = startAngle + pos * (endAngle - startAngle);

    // tick ring (engraved), amber for active portion
    const int numTicks = 11;
    for (int i = 0; i < numTicks; ++i)
    {
        const float t = (float) i / (float) (numTicks - 1);
        const float a = startAngle + t * (endAngle - startAngle);
        auto p1 = centre.getPointOnCircumference (radius * 1.03f, a);
        auto p2 = centre.getPointOnCircumference (radius * 1.16f, a);
        const bool major = (i == 0 || i == numTicks - 1 || i == (numTicks - 1) / 2);
        g.setColour (engrave.withAlpha (0.9f));
        g.drawLine (p1.x, p1.y, p2.x, p2.y, major ? 2.0f : 1.0f);
        if (a <= angle + 0.0001f)
        {
            g.setColour (amber.withAlpha (0.9f));
            g.drawLine (p1.x, p1.y, p2.x, p2.y, major ? 2.0f : 1.2f);
        }
    }

    // value arc with glow
    {
        const float arcR = radius * 0.96f;
        juce::Path track; track.addCentredArc (centre.x, centre.y, arcR, arcR, 0.0f, startAngle, endAngle, true);
        g.setColour (panelInset);
        g.strokePath (track, juce::PathStrokeType (3.5f));

        juce::Path arc; arc.addCentredArc (centre.x, centre.y, arcR, arcR, 0.0f, startAngle, angle, true);
        g.setColour (amber.withAlpha (0.25f));
        g.strokePath (arc, juce::PathStrokeType (8.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
        g.setColour (amberBright);
        g.strokePath (arc, juce::PathStrokeType (3.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }

    // drop shadow
    const float bodyR = radius * 0.74f;
    g.setColour (juce::Colours::black.withAlpha (0.45f));
    g.fillEllipse (juce::Rectangle<float> (bodyR * 2.0f, bodyR * 2.0f).withCentre (centre + juce::Point<float> (0.0f, 2.5f)));

    // dark skirt
    g.setColour (metalDark);
    g.fillEllipse (juce::Rectangle<float> (bodyR * 2.0f, bodyR * 2.0f).withCentre (centre));
    g.setColour (juce::Colours::black.withAlpha (0.35f));
    g.drawEllipse (juce::Rectangle<float> (bodyR * 2.0f, bodyR * 2.0f).withCentre (centre), 1.0f);

    // brushed metal cap (radial highlight)
    const float capR = radius * 0.58f;
    auto cap = juce::Rectangle<float> (capR * 2.0f, capR * 2.0f).withCentre (centre);
    juce::ColourGradient cg (metalLight, centre.x - capR * 0.45f, centre.y - capR * 0.6f,
                             metalDark,  centre.x + capR * 0.55f, centre.y + capR * 0.75f, true);
    cg.addColour (0.6, metalMid);
    g.setGradientFill (cg);
    g.fillEllipse (cap);
    g.setColour (juce::Colours::white.withAlpha (0.12f));
    g.drawEllipse (cap, 1.4f);
    g.setColour (juce::Colours::black.withAlpha (0.35f));
    g.drawEllipse (cap.expanded (1.0f), 1.0f);

    // amber pointer
    auto tip  = centre.getPointOnCircumference (capR * 0.95f, angle);
    auto base = centre.getPointOnCircumference (capR * 0.15f, angle);
    g.setColour (amber.withAlpha (0.35f));
    g.drawLine (base.x, base.y, tip.x, tip.y, 5.5f);
    g.setColour (amberBright);
    g.drawLine (base.x, base.y, tip.x, tip.y, 2.4f);
    g.setColour (amber.withAlpha (0.4f));
    g.fillEllipse (juce::Rectangle<float> (11.0f, 11.0f).withCentre (tip));
    g.setColour (amberBright);
    g.fillEllipse (juce::Rectangle<float> (5.0f, 5.0f).withCentre (tip));
}

// ── Linear slider: recessed slot, amber fill, brushed metal handle ────────────
void MC3LookAndFeel::drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                                       float sliderPos, float, float,
                                       juce::Slider::SliderStyle style, juce::Slider&)
{
    const bool horizontal = (style == juce::Slider::LinearHorizontal || style == juce::Slider::LinearBar);
    auto area = juce::Rectangle<int> (x, y, width, height).toFloat();

    if (horizontal)
    {
        auto track = juce::Rectangle<float> (area.getX(), area.getCentreY() - 4.0f, area.getWidth(), 8.0f);
        mc3::drawRecessedWell (g, track, 4.0f);

        auto fill = track.withWidth (sliderPos - track.getX()).reduced (0.0f, 1.5f);
        if (fill.getWidth() > 0.0f)
        {
            g.setColour (amber.withAlpha (0.25f));
            g.fillRoundedRectangle (fill.expanded (1.5f, 1.0f), 3.0f);
            g.setColour (amber);
            g.fillRoundedRectangle (fill, 3.0f);
        }

        // handle
        const float hw = 14.0f, hh = area.getHeight() - 4.0f;
        auto handle = juce::Rectangle<float> (hw, hh).withCentre ({ sliderPos, area.getCentreY() });
        mc3::drawBrushedMetal (g, handle, metalLight, metalDark, 3.0f);
        g.setColour (juce::Colours::black.withAlpha (0.4f));
        g.drawRoundedRectangle (handle, 3.0f, 1.0f);
        g.setColour (amberBright);
        g.drawLine (handle.getCentreX(), handle.getY() + 3.0f, handle.getCentreX(), handle.getBottom() - 3.0f, 1.5f);
    }
    else
    {
        auto track = juce::Rectangle<float> (area.getCentreX() - 4.0f, area.getY(), 8.0f, area.getHeight());
        mc3::drawRecessedWell (g, track, 4.0f);

        auto fill = juce::Rectangle<float> (track.getX(), sliderPos, track.getWidth(), track.getBottom() - sliderPos).reduced (1.5f, 0.0f);
        if (fill.getHeight() > 0.0f) { g.setColour (amber); g.fillRoundedRectangle (fill, 3.0f); }

        const float hw = area.getWidth() - 4.0f, hh = 14.0f;
        auto handle = juce::Rectangle<float> (hw, hh).withCentre ({ area.getCentreX(), sliderPos });
        mc3::drawBrushedMetal (g, handle, metalLight, metalDark, 3.0f);
        g.setColour (juce::Colours::black.withAlpha (0.4f));
        g.drawRoundedRectangle (handle, 3.0f, 1.0f);
    }
}

// ── Toggle = illuminated switch with amber LED ────────────────────────────────
void MC3LookAndFeel::drawToggleButton (juce::Graphics& g, juce::ToggleButton& b,
                                       bool highlighted, bool /*down*/)
{
    auto bounds = b.getLocalBounds().toFloat();
    const bool on = b.getToggleState();

    auto housing = bounds.removeFromLeft (juce::jmin (bounds.getHeight() * 1.9f, bounds.getWidth() * 0.5f)).reduced (2.0f);
    mc3::drawBrushedMetal (g, housing, metalMid, metalDark, 4.0f);
    g.setColour (juce::Colours::black.withAlpha (0.45f));
    g.drawRoundedRectangle (housing, 4.0f, 1.0f);

    // LED
    auto led = juce::Rectangle<float> (10.0f, 10.0f).withCentre ({ housing.getCentreX(), housing.getCentreY() });
    if (on)
    {
        g.setColour (amber.withAlpha (0.5f));
        g.fillEllipse (led.expanded (5.0f));
        g.setColour (amberBright);
    }
    else
    {
        g.setColour (juce::Colour (0xFF3a2a12));
    }
    g.fillEllipse (led);
    g.setColour (juce::Colours::black.withAlpha (0.4f));
    g.drawEllipse (led, 1.0f);

    // label
    auto textArea = bounds.toNearestInt().withTrimmedLeft (4);
    mc3::drawEngravedText (g, b.getButtonText(), textArea, juce::Justification::centredLeft,
                           MC3LookAndFeel::engravedFont (12.0f, true),
                           highlighted ? amberBright : textDim);
}

void MC3LookAndFeel::drawButtonBackground (juce::Graphics& g, juce::Button& b, const juce::Colour&,
                                           bool highlighted, bool down)
{
    auto bounds = b.getLocalBounds().toFloat().reduced (1.0f);
    auto top = down ? metalDark : (highlighted ? metalLight : metalMid);
    auto bot = down ? metalMid  : metalDark;
    mc3::drawBrushedMetal (g, bounds, top, bot, 4.0f);
    g.setColour (juce::Colours::black.withAlpha (0.45f));
    g.drawRoundedRectangle (bounds, 4.0f, 1.2f);
    g.setColour (juce::Colours::white.withAlpha (0.10f));
    g.drawLine (bounds.getX() + 4.0f, bounds.getY() + 1.0f, bounds.getRight() - 4.0f, bounds.getY() + 1.0f, 1.0f);
}

// ── ComboBox: recessed amber-lit readout ──────────────────────────────────────
void MC3LookAndFeel::drawComboBox (juce::Graphics& g, int width, int height, bool,
                                   int, int, int, int, juce::ComboBox&)
{
    auto bounds = juce::Rectangle<float> (0.0f, 0.0f, (float) width, (float) height).reduced (1.0f);
    mc3::drawRecessedWell (g, bounds, 4.0f);
    g.setColour (amber.withAlpha (0.25f));
    g.drawRoundedRectangle (bounds.reduced (0.5f), 4.0f, 1.0f);

    // arrow
    juce::Path p;
    const float cx = (float) width - 14.0f, cy = (float) height * 0.5f;
    p.addTriangle (cx - 5.0f, cy - 3.0f, cx + 5.0f, cy - 3.0f, cx, cy + 4.0f);
    g.setColour (amber);
    g.fillPath (p);
}

void MC3LookAndFeel::positionComboBoxText (juce::ComboBox& box, juce::Label& label)
{
    label.setBounds (10, 1, box.getWidth() - 30, box.getHeight() - 2);
    label.setFont (getComboBoxFont (box));
    label.setJustificationType (juce::Justification::centredLeft);
}

juce::Font MC3LookAndFeel::getComboBoxFont (juce::ComboBox&)      { return engravedFont (13.0f, true); }
juce::Font MC3LookAndFeel::getLabelFont (juce::Label&)            { return engravedFont (12.0f, false); }
juce::Font MC3LookAndFeel::getTextButtonFont (juce::TextButton&, int) { return engravedFont (12.0f, true); }

juce::Label* MC3LookAndFeel::createSliderTextBox (juce::Slider& slider)
{
    auto* l = LookAndFeel_V4::createSliderTextBox (slider);
    l->setFont (engravedFont (11.0f, true));
    l->setColour (juce::Label::textColourId, amberBright);
    l->setJustificationType (juce::Justification::centred);
    return l;
}
