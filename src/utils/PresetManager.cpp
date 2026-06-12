#include "PresetManager.h"
#include "../PluginProcessor.h"

PresetManager::PresetManager (MC3PluginAudioProcessor& p) : processor (p)
{
    presetsDirectory = getPresetsFolder();
    createPresetDirectory();
    scanForPresets();
    createFactoryPresets();
    scanForPresets();
}

juce::File PresetManager::getPresetsFolder()
{
    auto appSupport = juce::File::getSpecialLocation (juce::File::commonApplicationDataDirectory);
    return appSupport.getChildFile ("JTekk Audio").getChildFile ("MC-3 Mastering Compressor");
}

void PresetManager::createPresetDirectory()
{
    if (!presetsDirectory.exists())
        presetsDirectory.createDirectory();
}

void PresetManager::scanForPresets()
{
    presets.clear();
    currentPresetIndex = -1;

    auto xmlFiles = presetsDirectory.findChildFiles (juce::File::findFiles, false, "*.xml");

    for (const auto& file : xmlFiles)
    {
        Preset p;
        p.name = file.getFileNameWithoutExtension();
        p.file = file;
        presets.push_back (p);
    }

    std::sort (presets.begin(), presets.end(),
               [](const Preset& a, const Preset& b) { return a.name < b.name; });
}

void PresetManager::savePreset (const juce::String& presetName)
{
    const auto safeName = sanitizePresetName (presetName);
    const auto filePath = presetsDirectory.getChildFile (safeName + ".xml");

    juce::MemoryBlock data;
    processor.getStateInformation (data);

    if (filePath.replaceWithData (data.getData(), data.getSize()))
    {
        scanForPresets();

        // Find the index of the newly saved preset
        for (size_t i = 0; i < presets.size(); ++i)
        {
            if (presets[i].name == safeName)
            {
                currentPresetIndex = (int) i;
                break;
            }
        }
    }
}

void PresetManager::loadPreset (size_t index)
{
    if (index >= presets.size())
        return;

    const auto& preset = presets[index];

    if (auto xmlData = std::unique_ptr<juce::XmlElement> (juce::XmlDocument::parse (preset.file)))
    {
        const auto state = juce::ValueTree::fromXml (*xmlData);
        processor.getAPVTS().replaceState (state);
        currentPresetIndex = (int) index;
    }
}

void PresetManager::deletePreset (size_t index)
{
    if (index >= presets.size())
        return;

    const auto& preset = presets[index];
    if (preset.file.exists())
        preset.file.deleteFile();

    scanForPresets();
    currentPresetIndex = -1;
}

juce::String PresetManager::getCurrentPresetName() const
{
    if (currentPresetIndex >= 0 && currentPresetIndex < (int) presets.size())
        return presets[currentPresetIndex].name;
    return "Unsaved";
}

juce::String PresetManager::sanitizePresetName (const juce::String& name) const
{
    return name.retainCharacters ("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ");
}

void PresetManager::createFactoryPresets()
{
    struct FactoryPreset
    {
        const char* name;
        float fetThreshold, fetRatio, fetAttack, fetRelease, fetMakeupGain;
        float optoThreshold, optoRatio, optoAttack, optoRelease, optoMakeupGain;
        float eqLowGain, eqMidGain, eqHighGain;
        int inputTransformer, outputTransformer;
        bool useOversampling;
    };

    const FactoryPreset presets_[] = {
        // Default - balanced
        {"Default", -20.0f, 4.0f, 10.0f, 100.0f, 0.0f, -20.0f, 4.0f, 10.0f, 100.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 2, 2, true},

        // Bright - minimal coloration
        {"Bright", -18.0f, 3.0f, 5.0f, 80.0f, 0.0f, -22.0f, 3.5f, 8.0f, 90.0f, 0.0f,
         -2.0f, -1.0f, 2.0f, 1, 1, true},

        // Warm - colored with transformer saturation
        {"Warm", -22.0f, 5.0f, 15.0f, 150.0f, 2.0f, -20.0f, 4.5f, 12.0f, 120.0f, 1.5f,
         3.0f, 1.0f, 2.0f, 3, 3, true},

        // Aggressive - heavy compression
        {"Aggressive", -12.0f, 8.0f, 2.0f, 50.0f, 6.0f, -14.0f, 6.0f, 3.0f, 60.0f, 5.0f,
         1.0f, 0.0f, 1.0f, 2, 2, true},

        // Glue - transparent but cohesive
        {"Glue", -25.0f, 2.5f, 8.0f, 120.0f, 0.0f, -24.0f, 2.0f, 10.0f, 150.0f, 0.0f,
         0.5f, 0.0f, 0.5f, 2, 2, true},

        // Loud - pushes the mix forward
        {"Loud", -15.0f, 6.0f, 4.0f, 80.0f, 4.0f, -17.0f, 5.5f, 6.0f, 100.0f, 3.0f,
         2.0f, 1.5f, 2.0f, 2, 2, true},

        // Subtle - gentle touch
        {"Subtle", -30.0f, 1.5f, 20.0f, 200.0f, 0.0f, -28.0f, 1.5f, 25.0f, 250.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 2, 2, true},
    };

    for (const auto& fp : presets_)
    {
        auto presetFile = presetsDirectory.getChildFile (juce::String (fp.name) + ".xml");
        if (presetFile.exists())
            continue;  // Don't overwrite existing factory presets

        juce::XmlElement root ("Parameters");
        root.setAttribute ("fetThreshold", fp.fetThreshold);
        root.setAttribute ("fetRatio", fp.fetRatio);
        root.setAttribute ("fetAttack", fp.fetAttack);
        root.setAttribute ("fetRelease", fp.fetRelease);
        root.setAttribute ("fetMakeupGain", fp.fetMakeupGain);
        root.setAttribute ("optoThreshold", fp.optoThreshold);
        root.setAttribute ("optoRatio", fp.optoRatio);
        root.setAttribute ("optoAttack", fp.optoAttack);
        root.setAttribute ("optoRelease", fp.optoRelease);
        root.setAttribute ("optoMakeupGain", fp.optoMakeupGain);
        root.setAttribute ("eqLowGain", fp.eqLowGain);
        root.setAttribute ("eqMidGain", fp.eqMidGain);
        root.setAttribute ("eqHighGain", fp.eqHighGain);
        root.setAttribute ("inputTransformer", fp.inputTransformer);
        root.setAttribute ("outputTransformer", fp.outputTransformer);
        root.setAttribute ("useOversampling", fp.useOversampling ? 1 : 0);

        presetFile.replaceWithText (root.toString());
    }
}
