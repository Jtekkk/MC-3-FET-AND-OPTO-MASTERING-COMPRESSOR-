#pragma once

#include <juce_core/juce_core.h>
#include <vector>

class MC3PluginAudioProcessor;

struct Preset
{
    juce::String name;
    juce::File file;
};

class PresetManager
{
public:
    explicit PresetManager (MC3PluginAudioProcessor& processor);
    ~PresetManager() = default;

    // File operations
    void createPresetDirectory();
    void scanForPresets();
    void savePreset (const juce::String& presetName);
    void loadPreset (size_t index);
    void deletePreset (size_t index);

    // Query
    const std::vector<Preset>& getPresets() const { return presets; }
    int getCurrentPresetIndex() const { return currentPresetIndex; }
    juce::String getCurrentPresetName() const;

    // Factory presets
    void createFactoryPresets();

private:
    MC3PluginAudioProcessor& processor;
    std::vector<Preset> presets;
    int currentPresetIndex = -1;
    juce::File presetsDirectory;

    static juce::File getPresetsFolder();
    juce::String sanitizePresetName (const juce::String& name) const;
};
