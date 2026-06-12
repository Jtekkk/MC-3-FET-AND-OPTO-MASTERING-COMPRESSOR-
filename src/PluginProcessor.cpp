#include "PluginProcessor.h"
#include "PluginEditor.h"

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
    fetCompressor = std::make_unique<CompressorProcessor>(CompressorType::FET);
    optoCompressor = std::make_unique<CompressorProcessor>(CompressorType::OPTO);
    eqProcessor = std::make_unique<EQProcessor>();
    inputTransformer = std::make_unique<TransformerSimulation>(TransformerType::INPUT);
    outputTransformer = std::make_unique<TransformerSimulation>(TransformerType::OUTPUT);
    oversampler = std::make_unique<Oversampler>(8, sampleRate);

    fetCompressor->prepareToPlay (sampleRate, samplesPerBlock);
    optoCompressor->prepareToPlay (sampleRate, samplesPerBlock);
    eqProcessor->prepareToPlay (sampleRate, samplesPerBlock);
    inputTransformer->prepareToPlay (sampleRate, samplesPerBlock);
    outputTransformer->prepareToPlay (sampleRate, samplesPerBlock);
    oversampler->prepareToPlay (sampleRate, samplesPerBlock);
}

void MC3PluginAudioProcessor::releaseResources()
{
    fetCompressor.reset();
    optoCompressor.reset();
    eqProcessor.reset();
    inputTransformer.reset();
    outputTransformer.reset();
    oversampler.reset();
}

bool MC3PluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

void MC3PluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Get parameter values
    auto useOversampling = apvts.getRawParameterValue ("useOversampling")->load() > 0.5f;
    auto fetBypass = apvts.getRawParameterValue ("fetBypass")->load() > 0.5f;
    auto optoBypass = apvts.getRawParameterValue ("optoBypass")->load() > 0.5f;
    auto eqBypass = apvts.getRawParameterValue ("eqBypass")->load() > 0.5f;
    auto outputGain = apvts.getRawParameterValue ("outputGain")->load();

    // Process through input transformer
    inputTransformer->process (buffer);

    if (useOversampling)
    {
        auto* oversampledBuffer = oversampler->upsample (buffer);

        if (!fetBypass)
            fetCompressor->process (*oversampledBuffer);
        if (!optoBypass)
            optoCompressor->process (*oversampledBuffer);
        if (!eqBypass)
            eqProcessor->process (*oversampledBuffer);

        oversampler->downsample (*oversampledBuffer, buffer);
    }
    else
    {
        if (!fetBypass)
            fetCompressor->process (buffer);
        if (!optoBypass)
            optoCompressor->process (buffer);
        if (!eqBypass)
            eqProcessor->process (buffer);
    }

    // Process through output transformer
    outputTransformer->process (buffer);

    // Apply output gain
    buffer.applyGain (outputGain);
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

void MC3PluginAudioProcessor::valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyChanged,
                                                        const juce::Identifier& property)
{
    // Handle parameter changes for real-time updates
    if (treeWhosePropertyChanged == apvts.state)
    {
        // Real-time parameter updates can be handled here
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MC3PluginAudioProcessor();
}
