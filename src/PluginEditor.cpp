#include "PluginEditor.h"

namespace { constexpr int kDesignW = 1080, kDesignH = 720; }

MC3PluginAudioProcessorEditor::MC3PluginAudioProcessorEditor (MC3PluginAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    mainComponent = std::make_unique<MainComponent> (audioProcessor);
    addAndMakeVisible (mainComponent.get());

    // Resizable with a locked aspect ratio; the whole faceplate scales as one.
    setResizable (true, true);
    if (auto* c = getConstrainer())
        c->setFixedAspectRatio ((double) kDesignW / (double) kDesignH);
    setResizeLimits (kDesignW * 65 / 100, kDesignH * 65 / 100,
                     kDesignW * 3 / 2,     kDesignH * 3 / 2);

    setSize (kDesignW, kDesignH);
}

MC3PluginAudioProcessorEditor::~MC3PluginAudioProcessorEditor() {}

void MC3PluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void MC3PluginAudioProcessorEditor::resized()
{
    // Lay the main component out at the fixed design size, then scale it to fill.
    const float scale = (float) getWidth() / (float) kDesignW;
    mainComponent->setTransform (juce::AffineTransform::scale (scale));
    mainComponent->setBounds (0, 0, kDesignW, kDesignH);
}
