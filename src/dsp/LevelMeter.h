#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <atomic>

class LevelMeter
{
public:
    LevelMeter();
    ~LevelMeter() = default;

    void prepareToPlay (double sampleRate);
    void process (const juce::AudioBuffer<float>& buffer);

    float getCurrentLevelDb() const noexcept { return currentLevelDb.load(); }
    float getPeakLevelDb() const noexcept   { return peakLevelDb.load(); }
    bool  isClipping() const noexcept        { return isClipping_.load(); }

    void reset() noexcept;

private:
    double sampleRate = 0.0;
    std::atomic<float> currentLevelDb   { -80.0f };
    std::atomic<float> peakLevelDb      { -80.0f };
    std::atomic<bool>  isClipping_      { false };

    int peakHoldSamples = 0;
    int peakHoldCounter = 0;
    float peakHoldLevel = 0.0f;
};
