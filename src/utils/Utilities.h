#pragma once

#include <cmath>
#include <algorithm>

namespace MC3Utilities
{
    inline float dbToLinear (float db)
    {
        return std::pow (10.0f, db / 20.0f);
    }

    inline float linearToDb (float linear)
    {
        return 20.0f * std::log10 (std::max (linear, 1e-6f));
    }

    inline float timeConstantToCoefficient (float timeMs, float sampleRate)
    {
        return std::exp (-2.0f * 3.14159f / (timeMs * sampleRate * 0.001f));
    }

    inline float softClip (float x)
    {
        if (x < -1.5f) return -1.0f;
        if (x > 1.5f) return 1.0f;
        return x - (x * x * x / 3.0f);
    }

    inline float tanh (float x)
    {
        return std::tanh (x);
    }
}
