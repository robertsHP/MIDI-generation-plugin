/*
  ==============================================================================

    TrainingConfigComponent.cpp
    Created: 8 May 2024 11:50:22am
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TrainingConfigComponent.h"

//==============================================================================
TrainingConfigComponent::TrainingConfigComponent()
{
    // Initialize Training Group
    addAndMakeVisible(trainingGroup);
    trainingGroup.setText("Training Configurations");

    // Initialize Labels
    epochsLabel.setText("Epochs:", juce::dontSendNotification);
    batchSizeLabel.setText("Batch Size:", juce::dontSendNotification);
    sequenceLengthLabel.setText("Sequence Length:", juce::dontSendNotification);

    // Add Labels to Group
    trainingGroup.addAndMakeVisible(&epochsLabel);
    trainingGroup.addAndMakeVisible(&batchSizeLabel);
    trainingGroup.addAndMakeVisible(&sequenceLengthLabel);

    // Initialize Input Editors
    epochsEditor.setText("3");
    batchSizeEditor.setText("32");
    sequenceLengthEditor.setText("20");

    // Restrict input to numerical values only
    epochsEditor.setInputFilter(new NumericInputFilter(), false);
    batchSizeEditor.setInputFilter(new NumericInputFilter(), false);
    sequenceLengthEditor.setInputFilter(new NumericInputFilter(), false);

    // Add Input Editors to Group
    trainingGroup.addAndMakeVisible(&epochsEditor);
    trainingGroup.addAndMakeVisible(&batchSizeEditor);
    trainingGroup.addAndMakeVisible(&sequenceLengthEditor);

    // Add the group to the component
    addAndMakeVisible(&trainingGroup);
}

TrainingConfigComponent::~TrainingConfigComponent()
{
}

std::map<std::string, std::any> TrainingConfigComponent::getInputValuesAsMap()
{
    std::map<std::string, std::any> inputs;

    int epochs = epochsEditor.getText().getIntValue();
    inputs["epochs"] = epochs;

    int batchSize = batchSizeEditor.getText().getIntValue();
    inputs["batch_size"] = batchSize;

    int sequenceLength = sequenceLengthEditor.getText().getIntValue();
    inputs["sequence_length"] = sequenceLength;

    return inputs;
}

bool TrainingConfigComponent::areInputsValid()
{
    // Helper lambda to check if string is a valid integer and meets specific conditions
    auto isValidNumber = [](const juce::String& text, int min, int max) {
        return !text.isEmpty() && text.containsOnly("0123456789") &&
            text.getIntValue() >= min && text.getIntValue() <= max;
        };

    // Validate epochs: for example, valid if between 1 and 1000
    if (!isValidNumber(epochsEditor.getText(), 1, 1000))
        return false;

    // Validate batch size: for example, valid if between 1 and 1024
    if (!isValidNumber(batchSizeEditor.getText(), 1, 1024))
        return false;

    // Validate sequence length: for example, valid if between 1 and 500
    if (!isValidNumber(sequenceLengthEditor.getText(), 1, 500))
        return false;

    return true;
}

void TrainingConfigComponent::paint(juce::Graphics& g)
{
    // Commented out sample code. You can customize this.
    // g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
    // g.setColour (juce::Colours::grey);
    // g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    // g.setColour (juce::Colours::white);
    // g.setFont (14.0f);
    // g.drawText ("TrainingConfigComponent", getLocalBounds(),
    //             juce::Justification::centred, true);   // draw some placeholder text
}

void TrainingConfigComponent::resized()
{
    const int labelWidth = 100;
    const int editorHeight = 25;
    const int padding = 0;
    const int groupPadding = 15;

    const int trainingGroupHeight = 3 * (editorHeight + padding) + 2 * groupPadding;

    trainingGroup.setBounds(10, 10, getWidth() - 20, trainingGroupHeight);

    const int labelX = 10;
    const int editorX = labelX + labelWidth + padding;
    const int editorWidth = trainingGroup.getWidth() - labelWidth - 50;

    epochsLabel.setBounds(labelX, groupPadding, labelWidth, editorHeight);
    epochsEditor.setBounds(editorX, groupPadding, editorWidth, editorHeight);

    batchSizeLabel.setBounds(labelX, groupPadding + editorHeight + padding, labelWidth, editorHeight);
    batchSizeEditor.setBounds(editorX, groupPadding + editorHeight + padding, editorWidth, editorHeight);

    sequenceLengthLabel.setBounds(labelX, groupPadding + 2 * (editorHeight + padding), labelWidth, editorHeight);
    sequenceLengthEditor.setBounds(editorX, groupPadding + 2 * (editorHeight + padding), editorWidth, editorHeight);
}

int TrainingConfigComponent::getRequiredHeight()
{
    const int editorHeight = 25;
    const int padding = 10;
    const int groupPadding = 10;

    return 3 * (editorHeight + padding) + 2 * groupPadding;
}