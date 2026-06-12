#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include "../utils/ParameterSmoothing.h"
#include "SidechainFilter.h"
#include <deque>

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
    void setKneeWidth  (float db);
    void setLookahead  (float ms);
    void setDryWet     (float mix);  // 0.0 = dry, 1.0 = wet
    void setSidechainFreq (float hz);
    void setSidechainEnabled (bool enabled);

    float getGainReductionDb() const noexcept { return gainReductionDb; }

private:
    void updateCoefficientsIfNeeded();
    float calculateTimeConstant (float ms) noexcept;
    inline float detectPeak (const juce::AudioBuffer<float>& buffer, int startSample, int endSample) noexcept;
    inline float applyKnee (float gainDb, float knee) noexcept;
    inline float updateEnvelope (float peakDb) noexcept;

    CompressorType type;
    double sampleRate = 0.0;
    int lookaheadSamples = 0;
    std::deque<float> lookaheadBuffer;

    // Target values
    float thresholdTarget  = -20.0f;
    float ratioTarget      = 4.0f;
    float attackMsTarget   = 10.0f;
    float releaseMsTarget  = 100.0f;
    float makeupGainTarget = 0.0f;
    float kneeWidthTarget  = 0.0f;  // Hard knee
    float lookaheadMsTarget = 0.0f;
    float dryWetTarget     = 1.0f;
    float sidechainFreqTarget = 20.0f;
    bool sidechainEnabledTarget = false;

    // Smoothed parameters
    ParameterSmoother thresholdSmooth  { -20.0f };
    ParameterSmoother ratioSmooth      { 4.0f };
    ParameterSmoother kneeSmooth       { 0.0f };
    ParameterSmoother makeupGainSmooth { 0.0f };
    ParameterSmoother dryWetSmooth     { 1.0f };

    float threshold    = -20.0f;
    float ratio        = 4.0f;
    float attackMs     = 10.0f;
    float releaseMs    = 100.0f;
    float makeupGain   = 0.0f;
    float kneeWidth    = 0.0f;
    float dryWet       = 1.0f;

    float attackCoeff  = 0.0f;
    float releaseCoeff = 0.0f;
    float invRatio     = 0.25f;

    // State
    float envelope      = 0.0f;
    float gainReductionDb = 0.0f;

    // Type-specific time constant multipliers
    static constexpr float FET_ATTACK_MULT  = 0.8f;   // Faster attack
    static constexpr float OPTO_ATTACK_MULT = 1.3f;   // Slower attack
    static constexpr float FET_RELEASE_MULT = 0.7f;   // Faster release
    static constexpr float OPTO_RELEASE_MULT = 1.5f;  // Slower release

    SidechainFilter sidechainFilter;
    float sidechainFreq = 20.0f;
    bool sidechainEnabled = false;
};
