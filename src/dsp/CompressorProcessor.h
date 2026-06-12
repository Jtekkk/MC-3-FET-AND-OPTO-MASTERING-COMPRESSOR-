#pragma once

#include <juce_audio_basics/juce_audio_basics.h>

enum class CompressorType { FET, OPTO };

class CompressorProcessor
{
public:
    explicit CompressorProcessor (CompressorType type);
    ~CompressorProcessor() = default;

    void prepareToPlay (double sampleRate, int samplesPerBlock);
    void process (juce::AudioBuffer<float>& buffer);

    void setThreshold  (float db);
    void setRatio      (float ratio);
    void setAttack     (float ms);
    void setRelease    (float ms);
    void setMakeupGain (float db);

    float getGainReductionDb() const noexcept { return gainReductionDb; }

private:
    void updateCoefficients();

    CompressorType type;
    double sampleRate = 0.0;

    float threshold  = -20.0f;
    float ratio      = 4.0f;
    float attackMs   = 10.0f;
    float releaseMs  = 100.0f;
    float makeupGain = 0.0f;

    float attackCoeff  = 0.0f;
    float releaseCoeff = 0.0f;

    // Running state — single scalar, updated sample-by-sample
    float envelope      = 0.0f;
    float gainReductionDb = 0.0f;
};
