/*
  ==============================================================================

    TrainingComponent.cpp
    Created: 3 May 2024 3:03:40pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TrainingComponent.h"

#include "PluginProcessor.h"

//==============================================================================
TrainingComponent::TrainingComponent()
{
    addAndMakeVisible(listBoxComponent);
    listBoxComponent.setData("data", ListBoxComponent::Type::TRAINING);
    listBoxComponent.refreshFolders();

    addAndMakeVisible(trainingConfigComponent);
    addAndMakeVisible(modelConfigComponent);

    addAndMakeVisible(trainButton);
    trainButton.setButtonText("Train");
    trainButton.onClick = [this]() { trainButtonClicked(); };

    addAndMakeVisible(cancelButton);
    cancelButton.setButtonText("Cancel");
    cancelButton.onClick = [this]() { cancelButtonClicked(); };

    addAndMakeVisible(consoleComponent);
}

TrainingComponent::~TrainingComponent()
{
}

std::map<std::string, std::any> TrainingComponent::prepareInput() {
    juce::File pluginDirectory = PluginProcessor::getPluginDirectory();

    std::filesystem::path basePath = std::filesystem::absolute(pluginDirectory.getFullPathName().toStdString());

    std::map<std::string, std::any> training_config = trainingConfigComponent.getInputValuesAsMap();
    std::map<std::string, std::any> model_config = modelConfigComponent.getInputValuesAsMap();

    std::string selected_folder = listBoxComponent.getSelectedFolder().toStdString();

    std::map<std::string, std::any> inputArgs;

    inputArgs["selected_folder_name"] = selected_folder;
    inputArgs["data_path"] = (basePath / "data").string();
    inputArgs["model_path"] = (basePath / "models").string();
    inputArgs["training_config"] = training_config;
    inputArgs["model_config"] = model_config;

    return inputArgs;
}

void TrainingComponent::trainButtonClicked() {
    bool inputIsValid = trainingConfigComponent.areInputsValid() && modelConfigComponent.areInputsValid();
    bool folderSelected = listBoxComponent.isDirectorySelected();

    if (PluginProcessor::pythonBridge.taskRunning) {
        consoleComponent.appendText("Cannot perform action because a task is currently running!");
    } 
    else if (!inputIsValid) {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon, "Error", "Form inputs are not valid!");
    }
    else if (!folderSelected) {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon, "Error", "Folder is not selected!");
    }
    else {
        std::map<std::string, std::any> inputArgs = prepareInput();

        PluginProcessor::pythonBridge.callPythonTraining(inputArgs, consoleComponent);
    }
}

void TrainingComponent::cancelButtonClicked() {
    PluginProcessor::pythonBridge.cancelCurrentOperation(&consoleComponent);
}

void TrainingComponent::paint(juce::Graphics&) {

}

void TrainingComponent::resized()
{
    const int padding = 10;
    const int buttonHeight = 40;

    auto area = getLocalBounds();
    auto listBoxArea = area.removeFromLeft(proportionOfWidth(0.3));
    listBoxComponent.setBounds(listBoxArea);

    auto formArea = area.reduced(padding);

    auto trainingArea = formArea.removeFromTop(trainingConfigComponent.getRequiredHeight());
    trainingConfigComponent.setBounds(trainingArea);

    auto modelArea = formArea.removeFromTop(modelConfigComponent.getRequiredHeight());
    modelConfigComponent.setBounds(modelArea);

    auto buttonArea = formArea.removeFromTop(buttonHeight).reduced(padding);
    auto trainButtonArea = buttonArea.removeFromLeft(buttonArea.getWidth() / 2 - padding);
    trainButton.setBounds(trainButtonArea);

    auto cancelButtonArea = buttonArea;
    cancelButton.setBounds(cancelButtonArea);

    consoleComponent.setBounds(formArea.reduced(padding));
}
