#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "dsp/CompressorProcessor.h"
#include "dsp/EQProcessor.h"
#include "dsp/TransformerSimulation.h"
#include "dsp/Oversampler.h"
#include "dsp/LevelMeter.h"
#include "utils/Parameters.h"

class MC3PluginAudioProcessor : public juce::AudioProcessor,
                                 public juce::ValueTree::Listener
{
public:
    MC3PluginAudioProcessor();
    ~MC3PluginAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override;

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return "Default"; }
    void changeProgramName (int, const juce::String&) override {}

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState& getAPVTS() { return apvts; }
    const LevelMeter& getInputMeter()         const { return *inputMeter; }
    const LevelMeter& getOutputMeter()        const { return *outputMeter; }
    const CompressorProcessor& getFetCompressor()  const { return *fetCompressor; }
    const CompressorProcessor& getOptoCompressor() const { return *optoCompressor; }

private:
    void valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyChanged,
                                   const juce::Identifier& property) override;

    juce::AudioProcessorValueTreeState apvts;

    std::unique_ptr<CompressorProcessor> fetCompressor;
    std::unique_ptr<CompressorProcessor> optoCompressor;
    std::unique_ptr<EQProcessor> eqProcessor;
    std::unique_ptr<TransformerSimulation> inputTransformer;
    std::unique_ptr<TransformerSimulation> outputTransformer;
    std::unique_ptr<Oversampler> oversampler;
    std::unique_ptr<LevelMeter> inputMeter;
    std::unique_ptr<LevelMeter> outputMeter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MC3PluginAudioProcessor)
};
