#include "PresetPanel.h"
#include "../utils/PresetManager.h"

PresetPanel::PresetPanel (PresetManager& pm) : presetManager (pm)
{
    addAndMakeVisible (presetSelector);
    presetSelector.addListener (this);

    addAndMakeVisible (saveButton);
    saveButton.addListener (this);

    addAndMakeVisible (deleteButton);
    deleteButton.addListener (this);

    updatePresetList();
}

PresetPanel::~PresetPanel()
{
    presetSelector.removeListener (this);
    saveButton.removeListener (this);
    deleteButton.removeListener (this);
}

void PresetPanel::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xFF2a2a2a));

    g.setColour (juce::Colours::white);
    g.setFont (juce::Font (11.0f, juce::Font::bold));
    g.drawText ("PRESETS", 8, 6, 200, 16, juce::Justification::left);

    g.setColour (juce::Colour (0xFF444444));
    g.drawHorizontalLine (getHeight() - 1, 0.0f, (float) getWidth());
}

void PresetPanel::resized()
{
    auto b = getLocalBounds().reduced (8);
    b.removeFromTop (22);

    presetSelector.setBounds (b.removeFromLeft (b.getWidth() - 120).reduced (0, 4));
    saveButton.setBounds (b.removeFromLeft (55).reduced (2, 4));
    deleteButton.setBounds (b.reduced (2, 4));
}

void PresetPanel::comboBoxChanged (juce::ComboBox* comboBox)
{
    if (comboBox == &presetSelector)
    {
        const int selectedId = presetSelector.getSelectedId();
        if (selectedId > 0)
        {
            presetManager.loadPreset ((size_t) selectedId - 1);
        }
    }
}

void PresetPanel::buttonClicked (juce::Button* button)
{
    if (button == &saveButton)
    {
        showSaveDialog();
    }
    else if (button == &deleteButton)
    {
        confirmDelete();
    }
}

void PresetPanel::updatePresetList()
{
    presetSelector.clear (juce::dontSendNotification);

    const auto& presets = presetManager.getPresets();
    for (size_t i = 0; i < presets.size(); ++i)
    {
        presetSelector.addItem (presets[i].name, (int) (i + 1));
    }

    const auto currentIndex = presetManager.getCurrentPresetIndex();
    if (currentIndex >= 0 && currentIndex < (int) presets.size())
    {
        presetSelector.setSelectedId (currentIndex + 1, juce::dontSendNotification);
    }
}

void PresetPanel::showSaveDialog()
{
    auto options = juce::AlertWindow::getDefaultAlertWindow();

    juce::AlertWindow w ("Save Preset", "Enter preset name:", juce::AlertWindow::NoIcon);
    w.addTextEditor ("presetName", presetManager.getCurrentPresetName(), "Name:");
    w.addButton ("Save", 1, juce::KeyPress (juce::KeyPress::returnKey));
    w.addButton ("Cancel", 0, juce::KeyPress (juce::KeyPress::escapeKey));

    if (w.runModalLoop() != 0)
    {
        const auto presetName = w.getTextEditorContents ("presetName").trim();
        if (presetName.isNotEmpty())
        {
            presetManager.savePreset (presetName);
            updatePresetList();
        }
    }
}

void PresetPanel::confirmDelete()
{
    const auto currentIndex = presetManager.getCurrentPresetIndex();
    if (currentIndex < 0)
    {
        juce::AlertWindow::showMessageBoxAsync (
            juce::AlertWindow::InfoIcon,
            "Delete Preset",
            "No preset selected");
        return;
    }

    const auto& presets = presetManager.getPresets();
    if ((size_t) currentIndex >= presets.size())
        return;

    const auto presetName = presets[currentIndex].name;

    juce::AlertWindow w ("Delete Preset",
                         "Delete \"" + presetName + "\"?",
                         juce::AlertWindow::WarningIcon);
    w.addButton ("Delete", 1);
    w.addButton ("Cancel", 0, juce::KeyPress (juce::KeyPress::escapeKey));

    if (w.runModalLoop() != 0)
    {
        presetManager.deletePreset ((size_t) currentIndex);
        updatePresetList();
    }
}
