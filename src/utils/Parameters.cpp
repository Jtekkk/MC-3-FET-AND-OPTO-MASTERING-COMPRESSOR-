#include "Parameters.h"

juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    using Parameter = juce::AudioProcessorValueTreeState::Parameter;
    std::vector<std::unique_ptr<Parameter>> params;

    // FET Compressor Parameters
    params.push_back (std::make_unique<Parameter> (
        "fetThreshold", "FET Threshold", "",
        juce::NormalisableRange<float> (-60.0f, 0.0f), -20.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "fetRatio", "FET Ratio", "",
        juce::NormalisableRange<float> (1.0f, 20.0f), 4.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "fetAttack", "FET Attack", "",
        juce::NormalisableRange<float> (0.1f, 100.0f, 0.0f, 0.4f), 10.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "fetRelease", "FET Release", "",
        juce::NormalisableRange<float> (10.0f, 2000.0f, 0.0f, 0.3f), 100.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "fetMakeupGain", "FET Makeup Gain", "",
        juce::NormalisableRange<float> (0.0f, 60.0f), 0.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "fetBypass", "FET Bypass", "",
        juce::NormalisableRange<float> (0.0f, 1.0f), 0.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "fetKneeWidth", "FET Knee Width", "",
        juce::NormalisableRange<float> (0.0f, 12.0f), 0.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "fetLookahead", "FET Lookahead", "",
        juce::NormalisableRange<float> (0.0f, 50.0f), 0.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "fetSidechainFreq", "FET Sidechain Freq", "",
        juce::NormalisableRange<float> (20.0f, 5000.0f, 0.0f, 0.4f), 20.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "fetSidechainEnabled", "FET Sidechain Enabled", "",
        juce::NormalisableRange<float> (0.0f, 1.0f), 0.0f, nullptr, nullptr));

    // Opto Compressor Parameters
    params.push_back (std::make_unique<Parameter> (
        "optoThreshold", "Opto Threshold", "",
        juce::NormalisableRange<float> (-60.0f, 0.0f), -20.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "optoRatio", "Opto Ratio", "",
        juce::NormalisableRange<float> (1.0f, 20.0f), 4.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "optoAttack", "Opto Attack", "",
        juce::NormalisableRange<float> (0.1f, 100.0f, 0.0f, 0.4f), 10.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "optoRelease", "Opto Release", "",
        juce::NormalisableRange<float> (10.0f, 2000.0f, 0.0f, 0.3f), 100.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "optoMakeupGain", "Opto Makeup Gain", "",
        juce::NormalisableRange<float> (0.0f, 60.0f), 0.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "optoBypass", "Opto Bypass", "",
        juce::NormalisableRange<float> (0.0f, 1.0f), 0.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "optoKneeWidth", "Opto Knee Width", "",
        juce::NormalisableRange<float> (0.0f, 12.0f), 0.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "optoLookahead", "Opto Lookahead", "",
        juce::NormalisableRange<float> (0.0f, 50.0f), 0.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "optoSidechainFreq", "Opto Sidechain Freq", "",
        juce::NormalisableRange<float> (20.0f, 5000.0f, 0.0f, 0.4f), 20.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "optoSidechainEnabled", "Opto Sidechain Enabled", "",
        juce::NormalisableRange<float> (0.0f, 1.0f), 0.0f, nullptr, nullptr));

    // EQ Parameters
    params.push_back (std::make_unique<Parameter> (
        "eqLowGain", "EQ Low Gain", "",
        juce::NormalisableRange<float> (-12.0f, 12.0f), 0.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "eqMidGain", "EQ Mid Gain", "",
        juce::NormalisableRange<float> (-12.0f, 12.0f), 0.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "eqHighGain", "EQ High Gain", "",
        juce::NormalisableRange<float> (-12.0f, 12.0f), 0.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "eqBypass", "EQ Bypass", "",
        juce::NormalisableRange<float> (0.0f, 1.0f), 0.0f, nullptr, nullptr));

    // Transformer Selection — range 1–3 matches ComboBox item IDs
    params.push_back (std::make_unique<Parameter> (
        "inputTransformer", "Input Transformer", "",
        juce::NormalisableRange<float> (1.0f, 3.0f, 1.0f), 2.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "outputTransformer", "Output Transformer", "",
        juce::NormalisableRange<float> (1.0f, 3.0f, 1.0f), 2.0f, nullptr, nullptr));

    // Oversampling and Output
    params.push_back (std::make_unique<Parameter> (
        "useOversampling", "Use Oversampling", "",
        juce::NormalisableRange<float> (0.0f, 1.0f), 1.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "dryWetMix", "Dry/Wet Mix", "",
        juce::NormalisableRange<float> (0.0f, 1.0f), 1.0f, nullptr, nullptr));

    params.push_back (std::make_unique<Parameter> (
        "outputGain", "Output Gain", "",
        juce::NormalisableRange<float> (-12.0f, 12.0f), 0.0f, nullptr, nullptr));

    return { params.begin(), params.end() };
}
