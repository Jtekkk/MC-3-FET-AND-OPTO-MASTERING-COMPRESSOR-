cmake_minimum_required(VERSION 3.21)
project(MC3MasteringCompressor VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE
add_subdirectory(JUCE)

# Main plugin target
juce_add_plugin(MC3MasteringCompressor
    COMPANY_NAME "JTekk Audio"
    BUNDLE_ID "com.jtekkaudio.mc3masteringcompressor"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE JTA1
    PLUGIN_CODE MC3M
    FORMATS VST3 Standalone
    PRODUCT_NAME "MC-3 FET and Opto Mastering Compressor"
)

target_sources(MC3MasteringCompressor PRIVATE
    # Main plugin
    src/PluginProcessor.cpp
    src/PluginProcessor.h
    src/PluginEditor.cpp
    src/PluginEditor.h

    # DSP Processors
    src/dsp/CompressorProcessor.cpp
    src/dsp/CompressorProcessor.h
    src/dsp/EQProcessor.cpp
    src/dsp/EQProcessor.h
    src/dsp/TransformerSimulation.cpp
    src/dsp/TransformerSimulation.h
    src/dsp/Oversampler.cpp
    src/dsp/Oversampler.h
    src/dsp/LevelMeter.cpp
    src/dsp/LevelMeter.h
    src/dsp/SidechainFilter.cpp
    src/dsp/SidechainFilter.h

    # GUI Components
    src/ui/MainComponent.cpp
    src/ui/MainComponent.h
    src/ui/CompressorPanel.cpp
    src/ui/CompressorPanel.h
    src/ui/EQPanel.cpp
    src/ui/EQPanel.h
    src/ui/ControlsPanel.cpp
    src/ui/ControlsPanel.h
    src/ui/MeterComponent.cpp
    src/ui/MeterComponent.h
    src/ui/PresetPanel.cpp
    src/ui/PresetPanel.h

    # Utilities
    src/utils/Parameters.cpp
    src/utils/Parameters.h
    src/utils/ParameterSmoothing.h
    src/utils/PresetManager.cpp
    src/utils/PresetManager.h
    src/utils/Utilities.cpp
    src/utils/Utilities.h
)

target_compile_definitions(MC3MasteringCompressor PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(MC3MasteringCompressor PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
)
