/*
  ==============================================================================

    ListBoxComponent.cpp
    Created: 3 May 2024 4:45:27pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ListBoxComponent.h"

#include "PluginProcessor.h"

//==============================================================================
ListBoxComponent::ListBoxComponent()
{
    // Configure the Refresh button
    refreshButton.setButtonText("Refresh");
    refreshButton.onClick = [this]() { refreshButtonClicked(); };

    // Set the model for the list box
    listBox.setModel(this);
    listBox.setRowHeight(30);

    // Add the list box and button to the component
    addAndMakeVisible(refreshButton);
    addAndMakeVisible(listBox);
}

ListBoxComponent::~ListBoxComponent()
{
}

bool ListBoxComponent::isDirectorySelected() {
    return selectedItem != "";
}

juce::String ListBoxComponent::getSelectedFolder() {
	return selectedItem;
}

void ListBoxComponent::setData(const juce::String& name, Type type) {
    this->dirName = name;
    this->type = type;
}

void ListBoxComponent::refreshFolders()
{
    auto folderNames = getFolderNames();
    setListItems(folderNames);
}

std::vector<juce::String> ListBoxComponent::getFolderNames()
{
    std::vector<juce::String> folderNames;
    juce::File pluginDirectory = PluginProcessor::getPluginDirectory();
    std::filesystem::path basePath = std::filesystem::absolute(pluginDirectory.getFullPathName().toStdString());

    auto folder = juce::File(basePath.string() + "/" + dirName.toStdString());

    if (folder.exists() && folder.isDirectory()) {
        auto subdirectories = folder.findChildFiles(juce::File::findDirectories, false);

        for (const auto& subdirectory : subdirectories) {
            if (checkFiles(subdirectory)) {
                folderNames.push_back(subdirectory.getFileName());
            }
        }
    }
    return folderNames;
}

bool ListBoxComponent::checkFiles(const juce::File& directory) {
    if (type == Type::TRAINING) {
        return directory.findChildFiles(juce::File::findFiles, false, "*.midi;*.mid").size() > 0;
    }
    else if (type == Type::GENERATING) {
        auto pklFiles = directory.findChildFiles(juce::File::findFiles, false, "*.pkl").size() > 0;
        auto pthFiles = directory.findChildFiles(juce::File::findFiles, false, "*.pth").size() > 0;
        auto configJsonFiles = directory.findChildFiles(juce::File::findFiles, false, "*.config.json").size() > 0;
        return pklFiles && pthFiles && configJsonFiles;
    }
    return false;
}

void ListBoxComponent::setListItems(const std::vector<juce::String>& newItems) {
    listItems = newItems;
    listBox.updateContent();
    if (!listItems.empty()) {
        listBox.selectRow(0);
        selectedItem = listItems[0];
    }
}

void ListBoxComponent::refreshButtonClicked() {
    if (PluginProcessor::pythonBridge.taskRunning) {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon, "Error", "Cannot perform action because a task is currently running!");
    }
    else {
        refreshFolders();
    }
}

void ListBoxComponent::resized()
{
    auto area = getLocalBounds();
    auto buttonHeight = 50;
    refreshButton.setBounds(area.removeFromTop(buttonHeight).reduced(5));
    listBox.setBounds(area.reduced(5));
}

// ListBoxModel implementation
int ListBoxComponent::getNumRows()
{
    return static_cast<int>(listItems.size());
}

void ListBoxComponent::paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
        g.fillAll(juce::Colours::lightblue);
    else
        g.fillAll(juce::Colours::cadetblue);

    g.setColour(juce::Colours::black);
    g.drawText(listItems[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
}

void ListBoxComponent::listBoxItemClicked(int row, const juce::MouseEvent&)
{
    selectedItem = listItems[row];
}