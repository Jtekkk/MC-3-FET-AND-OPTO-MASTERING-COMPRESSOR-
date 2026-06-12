#include "EQProcessor.h"

EQProcessor::EQProcessor()
{
}

void EQProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = 2;

    lowShelf.prepare (spec);
    midPeak.prepare (spec);
    highShelf.prepare (spec);

    updateLowShelf();
    updateMidPeak();
    updateHighShelf();
}

void EQProcessor::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);

    lowShelf.process (context);
    midPeak.process (context);
    highShelf.process (context);
}

void EQProcessor::setLowGain (float db)
{
    lowGain = db;
    updateLowShelf();
}

void EQProcessor::setMidGain (float db)
{
    midGain = db;
    updateMidPeak();
}

void EQProcessor::setHighGain (float db)
{
    highGain = db;
    updateHighShelf();
}

void EQProcessor::updateLowShelf()
{
    *lowShelf.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf (sampleRate, 200.0f, 0.707f, lowGain);
}

void EQProcessor::updateMidPeak()
{
    *midPeak.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, 2000.0f, 0.707f, midGain);
}

void EQProcessor::updateHighShelf()
{
    *highShelf.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf (sampleRate, 8000.0f, 0.707f, highGain);
}
