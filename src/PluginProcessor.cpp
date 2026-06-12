#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "utils/Utilities.h"

MC3PluginAudioProcessor::MC3PluginAudioProcessor()
    : AudioProcessor (BusesProperties()
        .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
        .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
    , apvts (*this, nullptr, "Parameters", createParameterLayout())
{
    apvts.state.addListener (this);
}

MC3PluginAudioProcessor::~MC3PluginAudioProcessor()
{
    apvts.state.removeListener (this);
}

void MC3PluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    fetCompressor    = std::make_unique<CompressorProcessor> (CompressorType::FET);
    optoCompressor   = std::make_unique<CompressorProcessor> (CompressorType::OPTO);
    eqProcessor      = std::make_unique<EQProcessor>();
    inputTransformer  = std::make_unique<TransformerSimulation> (TransformerType::INPUT);
    outputTransformer = std::make_unique<TransformerSimulation> (TransformerType::OUTPUT);
    oversampler      = std::make_unique<Oversampler> (8, sampleRate);
    inputMeter       = std::make_unique<LevelMeter>();
    outputMeter      = std::make_unique<LevelMeter>();

    fetCompressor->prepareToPlay    (sampleRate, samplesPerBlock);
    optoCompressor->prepareToPlay   (sampleRate, samplesPerBlock);
    eqProcessor->prepareToPlay      (sampleRate, samplesPerBlock);
    inputTransformer->prepareToPlay  (sampleRate, samplesPerBlock);
    outputTransformer->prepareToPlay (sampleRate, samplesPerBlock);
    oversampler->prepareToPlay       (sampleRate, samplesPerBlock);
    inputMeter->prepareToPlay        (sampleRate);
    outputMeter->prepareToPlay       (sampleRate);
}

void MC3PluginAudioProcessor::releaseResources()
{
    fetCompressor.reset();
    optoCompressor.reset();
    eqProcessor.reset();
    inputTransformer.reset();
    outputTransformer.reset();
    oversampler.reset();
    inputMeter.reset();
    outputMeter.reset();
}

bool MC3PluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    return layouts.getMainOutputChannelSet() == layouts.getMainInputChannelSet();
}

// Helper to load one raw parameter value
static inline float param (juce::AudioProcessorValueTreeState& apvts, const char* id)
{
    return apvts.getRawParameterValue (id)->load();
}

void MC3PluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    for (int i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // ── Measure input level ───────────────────────────────────────────────
    inputMeter->process (buffer);

    // ── Push current parameter values to DSP objects ──────────────────────
    fetCompressor->setThreshold  (param (apvts, "fetThreshold"));
    fetCompressor->setRatio      (param (apvts, "fetRatio"));
    fetCompressor->setAttack     (param (apvts, "fetAttack"));
    fetCompressor->setRelease    (param (apvts, "fetRelease"));
    fetCompressor->setMakeupGain (param (apvts, "fetMakeupGain"));

    optoCompressor->setThreshold  (param (apvts, "optoThreshold"));
    optoCompressor->setRatio      (param (apvts, "optoRatio"));
    optoCompressor->setAttack     (param (apvts, "optoAttack"));
    optoCompressor->setRelease    (param (apvts, "optoRelease"));
    optoCompressor->setMakeupGain (param (apvts, "optoMakeupGain"));

    eqProcessor->setLowGain  (param (apvts, "eqLowGain"));
    eqProcessor->setMidGain  (param (apvts, "eqMidGain"));
    eqProcessor->setHighGain (param (apvts, "eqHighGain"));

    // ComboBox IDs are 1/2/3 → transformer index 0/1/2
    inputTransformer->setType  ((int) param (apvts, "inputTransformer")  - 1);
    outputTransformer->setType ((int) param (apvts, "outputTransformer") - 1);

    // ── Bypass / oversampling flags ───────────────────────────────────────
    const bool useOversampling = param (apvts, "useOversampling") > 0.5f;
    const bool fetBypass       = param (apvts, "fetBypass")       > 0.5f;
    const bool optoBypass      = param (apvts, "optoBypass")      > 0.5f;
    const bool eqBypass        = param (apvts, "eqBypass")        > 0.5f;
    const float outputGainDb   = param (apvts, "outputGain");

    // ── Signal chain ──────────────────────────────────────────────────────
    inputTransformer->process (buffer);

    auto process = [&](juce::AudioBuffer<float>& buf)
    {
        if (!fetBypass)  fetCompressor->process  (buf);
        if (!optoBypass) optoCompressor->process (buf);
        if (!eqBypass)   eqProcessor->process    (buf);
    };

    if (useOversampling)
    {
        auto* oversampledBuf = oversampler->upsample (buffer);
        process (*oversampledBuf);
        oversampler->downsample (*oversampledBuf, buffer);
    }
    else
    {
        process (buffer);
    }

    outputTransformer->process (buffer);

    // Convert output gain from dB to linear before applying
    buffer.applyGain (MC3Utilities::dbToLinear (outputGainDb));

    // ── Measure output level ──────────────────────────────────────────────
    outputMeter->process (buffer);
}

juce::AudioProcessorEditor* MC3PluginAudioProcessor::createEditor()
{
    return new MC3PluginAudioProcessorEditor (*this);
}

const juce::String MC3PluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

void MC3PluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyStateAsXml();
    copyXmlToBinary (*state, destData);
}

void MC3PluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    auto xmlState = getXmlFromBinary (data, sizeInBytes);
    if (xmlState != nullptr && xmlState->hasTagName (apvts.state.getType()))
        apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

void MC3PluginAudioProcessor::valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) {}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MC3PluginAudioProcessor();
}
