/*
  ==============================================================================

    GeneratingComponent.cpp
    Created: 3 May 2024 3:03:51pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GeneratingComponent.h"

#include "PluginProcessor.h"

//==============================================================================
GeneratingComponent::GeneratingComponent()
{
    //addAndMakeVisible(midiVisualizerComponent);
    addAndMakeVisible(listBoxComponent);
    listBoxComponent.setData("models", ListBoxComponent::Type::GENERATING);
    listBoxComponent.refreshFolders();

    addAndMakeVisible(midiDropComponent);
    addAndMakeVisible(generateButton);
    addAndMakeVisible(midiDragComponent);

    generateButton.setButtonText("Generate");
    generateButton.onClick = [this]() { generateButtonClicked(); };

    // Add ConsoleComponent
    addAndMakeVisible(consoleComponent);
}

GeneratingComponent::~GeneratingComponent()
{
}

std::map<std::string, std::any> GeneratingComponent::prepareInput() {
    juce::File pluginDirectory = PluginProcessor::getPluginDirectory();

    std::filesystem::path basePath = std::filesystem::absolute(pluginDirectory.getFullPathName().toStdString());
    std::string selected_folder = listBoxComponent.getSelectedFolder().toStdString();

    std::map<std::string, std::any> inputArgs;
    inputArgs["selected_folder_name"] = selected_folder;
    inputArgs["data_path"] = (basePath / "data").string();
    inputArgs["model_path"] = (basePath / "models").string();

    if (midiDropComponent.isFileSet()) {
        juce::MidiFile midiFile = midiDropComponent.getMidiFile();

        std::string tempMidiFilePath = (basePath / "models" / selected_folder / "temp_gen_midi.midi").string();
        std::replace(tempMidiFilePath.begin(), tempMidiFilePath.end(), '\\', '/');

        if (std::filesystem::exists(tempMidiFilePath)) {
            std::filesystem::remove(tempMidiFilePath);
        }

        juce::File tempMidiFile(tempMidiFilePath);
        juce::FileOutputStream outputStream(tempMidiFile);
        midiFile.writeTo(outputStream);

        inputArgs["temp_midi_file_path"] = tempMidiFilePath;
    }

    return inputArgs;
}

void GeneratingComponent::generateButtonClicked() {
    bool isInputFileSet = midiDropComponent.isFileSet();
    bool folderSelected = listBoxComponent.isDirectorySelected();

    if (PluginProcessor::pythonBridge.taskRunning) {
        consoleComponent.appendText("Cannot perform action because a task is currently running!");
    } 
    else if (!isInputFileSet) {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon, "Error", "File not set!");
    }
    else if (!folderSelected) {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon, "Error", "Folder is not selected!");
    }
    else {
        std::map<std::string, std::any> inputArgs = prepareInput();

        PluginProcessor::pythonBridge.callPythonGeneration(inputArgs, consoleComponent, midiDragComponent);
    }
}

void GeneratingComponent::paint (juce::Graphics& g)
{
    //g.fillAll(juce::Colours::defa;
    //g.drawText("Component A", getLocalBounds(), juce::Justification::centred, true);
}

void GeneratingComponent::resized()
{
    auto area = getLocalBounds();

    // Allocate specific heights to different areas
    auto topArea = area.removeFromTop(area.getHeight() / 9); // Reduced height for top components

    // Remaining area split into left for listBoxComponent and right for ConsoleComponent
    auto listAndConsoleArea = area;
    auto listBoxWidth = listAndConsoleArea.getWidth() * 2 / 3; // Two-thirds width for ListBoxComponent
    auto consoleWidth = listAndConsoleArea.getWidth() - listBoxWidth; // Remaining width for ConsoleComponent

    auto listBoxArea = listAndConsoleArea.removeFromLeft(listBoxWidth);
    auto consoleArea = listAndConsoleArea; // Remaining area

    // Configure the top area with midiDropComponent, generateButton, and midiDragComponent
    auto midiDropComponentWidth = topArea.getWidth() / 2.5;
    auto midiGrabComponentWidth = topArea.getWidth() / 2.5;
    auto generateButtonWidth = topArea.getWidth() - midiDropComponentWidth - midiGrabComponentWidth;

    midiDropComponent.setBounds(topArea.removeFromLeft(midiDropComponentWidth));
    generateButton.setBounds(topArea.removeFromLeft(generateButtonWidth));
    midiDragComponent.setBounds(topArea);

    // Set the listBoxComponent to occupy the entire left side area
    listBoxComponent.setBounds(listBoxArea);

    // Set the ConsoleComponent to occupy the entire right side area
    consoleComponent.setBounds(consoleArea);
}
