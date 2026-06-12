#pragma once

#include <cmath>

class ParameterSmoother
{
public:
    ParameterSmoother() = default;
    explicit ParameterSmoother (float initialValue) : current (initialValue), target (initialValue) {}

    void setTargetValue (float newTarget) noexcept
    {
        target = newTarget;
    }

    float getNextValue() noexcept
    {
        if (std::abs (current - target) < 1e-6f)
        {
            current = target;
            return current;
        }

        // One-pole low-pass filter with ~50ms smoothing at 48kHz
        current = smoothCoeff * current + (1.0f - smoothCoeff) * target;
        return current;
    }

    void reset (float value = 0.0f) noexcept
    {
        current = target = value;
    }

    float getCurrentValue() const noexcept { return current; }

private:
    static constexpr float smoothCoeff = 0.9985f;  // ~50ms @ 48kHz
    float current = 0.0f;
    float target = 0.0f;
};
