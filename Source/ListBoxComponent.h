/*
  ==============================================================================

    ListBoxComponent.h
    Created: 3 May 2024 4:45:27pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include <filesystem>

//==============================================================================
/*
*/
class ListBoxComponent : public juce::Component, public juce::ListBoxModel
{
public:
    enum Type {
        TRAINING, GENERATING
    };

    ListBoxComponent();
    ~ListBoxComponent() override;

    bool isDirectorySelected();
    juce::String getSelectedFolder();
    void setData(const juce::String& name, Type type);

    void refreshFolders();
    std::vector<juce::String> getFolderNames();
    bool checkFiles(const juce::File& directory);
    void setListItems(const std::vector<juce::String>& newItems);

    void refreshButtonClicked();

    void resized() override;
    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void listBoxItemClicked(int row, const juce::MouseEvent&) override;

private:

    juce::ListBox listBox{ "List Box", this };
    juce::TextButton refreshButton;
    std::vector<juce::String> listItems;

    juce::String dirName = "";
    juce::String selectedItem = "";

    Type type;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ListBoxComponent)
};
