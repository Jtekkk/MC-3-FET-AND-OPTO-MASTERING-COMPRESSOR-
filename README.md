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
- A C++17 compiler: **MSVC** on Windows, GCC/Clang on Linux, Apple Clang on macOS
- JUCE 8.0.13 (fetched into `./JUCE`)

### Build Instructions

JUCE is not vendored in the repo. Clone it next to the sources first (the
`CMakeLists.txt` does `add_subdirectory(JUCE)`):

```bash
git clone --depth 1 --branch 8.0.13 https://github.com/juce-framework/JUCE.git
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release \
  --target MC3MasteringCompressor_VST3 MC3MasteringCompressor_Standalone
```

Artifacts land in `build/MC3MasteringCompressor_artefacts/Release/` (`VST3/` and
`Standalone/`).

On Linux, install the JUCE dependencies first:

```bash
sudo apt-get install -y libasound2-dev libxinerama-dev libxrandr-dev \
  libxcursor-dev libxcomposite-dev libfreetype6-dev libfontconfig1-dev \
  libcurl4-openssl-dev
```

### Windows builds

JUCE **does not support MinGW** (there is a hard `#error` in
`juce_core/system/juce_TargetPlatform.h`), so a Windows plugin **cannot** be
cross-compiled from Linux — it must be built with MSVC on Windows. The
`.github/workflows/build.yml` workflow builds the Windows VST3 + Standalone on a
`windows-latest` (MSVC) runner and uploads them as artifacts; run it locally on
Windows with the commands above, or via GitHub Actions.

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
