#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class PresetManager;

class PresetPanel : public juce::Component,
                    public juce::ComboBox::Listener,
                    public juce::Button::Listener
{
public:
    explicit PresetPanel (PresetManager& presetManager);
    ~PresetPanel() override;

    void paint (juce::Graphics& g) override;
    void resized() override;

    void comboBoxChanged (juce::ComboBox* comboBox) override;
    void buttonClicked (juce::Button* button) override;

private:
    PresetManager& presetManager;

    juce::ComboBox presetSelector;
    juce::TextButton saveButton { "Save" };
    juce::TextButton deleteButton { "Delete" };

    void updatePresetList();
    void showSaveDialog();
    void confirmDelete();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PresetPanel)
};
