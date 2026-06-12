# MC-3 FET and Opto Mastering Compressor

A professional audio plugin featuring dual compressors (FET and Opto), 3-band EQ, selectable transformer models, and 8x oversampling.

## Features

- **Dual Compressors**: Independent FET and Opto compressor stages
- **3-Band EQ**: Low (200Hz), Mid (2kHz), and High (8kHz) shelves/peaks
- **Transformer Selection**: 3 selectable transformer models for input and output
  - Bright: Linear, minimal coloration
  - Neutral: Balanced transformer characteristics
  - Warm: Colored with harmonic saturation
- **8x Oversampling**: Optional high-quality oversampling for reduced aliasing
- **Professional Controls**: 
  - Threshold, Ratio, Attack, Release, Makeup Gain for each compressor
  - Bypass switches for all processing stages
  - Output gain control

## Building

### Requirements
- CMake 3.21+
- C++17 compatible compiler
- JUCE framework

### Build Instructions

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Project Structure

```
src/
├── PluginProcessor.h/cpp      # Main plugin processor
├── PluginEditor.h/cpp         # Editor window
├── dsp/                        # Audio processing modules
│   ├── CompressorProcessor.*   # FET/Opto compressor implementation
│   ├── EQProcessor.*           # 3-band EQ processor
│   ├── TransformerSimulation.* # Transformer model simulation
│   └── Oversampler.*           # 8x oversampling
├── ui/                         # GUI components
│   ├── MainComponent.*         # Main UI container
│   ├── CompressorPanel.*       # Compressor controls
│   ├── EQPanel.*               # EQ controls
│   └── ControlsPanel.*         # Global controls
└── utils/                      # Utilities
    ├── Parameters.*            # Parameter definitions
    └── Utilities.*             # Helper functions
```

## Next Steps

1. Connect parameter bindings between UI and DSP processors
2. Implement meter/visualization components
3. Add preset management system
4. Optimize DSP for real-time performance
5. Add factory presets
