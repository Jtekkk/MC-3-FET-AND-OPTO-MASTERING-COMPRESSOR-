#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include "../utils/ParameterSmoothing.h"

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
    void updateCoefficientsIfNeeded();
    inline float detectPeak (const juce::AudioBuffer<float>& buffer) noexcept;
    inline float updateEnvelope (float peakDb) noexcept;

    CompressorType type;
    double sampleRate = 0.0;

    // Target values (from UI)
    float thresholdTarget  = -20.0f;
    float ratioTarget      = 4.0f;
    float attackMsTarget   = 10.0f;
    float releaseMsTarget  = 100.0f;
    float makeupGainTarget = 0.0f;

    // Smoothed parameter values
    ParameterSmoother thresholdSmooth  { -20.0f };
    ParameterSmoother ratioSmooth      { 4.0f };
    ParameterSmoother makeupGainSmooth { 0.0f };

    float threshold    = -20.0f;
    float ratio        = 4.0f;
    float attackMs     = 10.0f;
    float releaseMs    = 100.0f;
    float makeupGain   = 0.0f;

    float attackCoeff  = 0.0f;
    float releaseCoeff = 0.0f;

    // Running state
    float envelope      = 0.0f;
    float gainReductionDb = 0.0f;

    // Optimization: cache inverse ratio
    float invRatio = 0.25f;
};
