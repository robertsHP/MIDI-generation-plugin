/*
  ==============================================================================

    ModelConfigComponent.cpp
    Created: 8 May 2024 11:50:04am
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ModelConfigComponent.h"

//==============================================================================
ModelConfigComponent::ModelConfigComponent()
{
    addAndMakeVisible(modelGroup);
    modelGroup.setText("Model Configurations");

    nInpLabel.setText("Input dim.:", juce::dontSendNotification);
    nHeadLabel.setText("Attention Heads:", juce::dontSendNotification);
    nHidLabel.setText("Feed-forward dim.:", juce::dontSendNotification);
    nLayersLabel.setText("Hidden layers:", juce::dontSendNotification);

    modelGroup.addAndMakeVisible(&nInpLabel);
    modelGroup.addAndMakeVisible(&nHeadLabel);
    modelGroup.addAndMakeVisible(&nHidLabel);
    modelGroup.addAndMakeVisible(&nLayersLabel);

    nInpEditor.setText("512");
    nHeadEditor.setText("8");
    nHidEditor.setText("512");
    nLayersEditor.setText("2");

    nInpEditor.setInputFilter(new NumericInputFilter(), false);
    nHeadEditor.setInputFilter(new NumericInputFilter(), false);
    nHidEditor.setInputFilter(new NumericInputFilter(), false);
    nLayersEditor.setInputFilter(new NumericInputFilter(), false);

    modelGroup.addAndMakeVisible(&nInpEditor);
    modelGroup.addAndMakeVisible(&nHeadEditor);
    modelGroup.addAndMakeVisible(&nHidEditor);
    modelGroup.addAndMakeVisible(&nLayersEditor);

    addAndMakeVisible(&modelGroup);
}

ModelConfigComponent::~ModelConfigComponent()
{
}

std::map<std::string, std::any> ModelConfigComponent::getInputValuesAsMap()
{
    std::map<std::string, std::any> inputs;

    int nInp = nInpEditor.getText().getIntValue();
    inputs["input_dim"] = nInp;

    int nHead = nHeadEditor.getText().getIntValue();
    inputs["attention_heads"] = nHead;

    int nHid = nHidEditor.getText().getIntValue();
    inputs["feed_forward_dim"] = nHid;

    int nLayers = nLayersEditor.getText().getIntValue();
    inputs["hidden_layers"] = nLayers;

    return inputs;
}

bool ModelConfigComponent::areInputsValid()
{
    if (nInpEditor.getText().isEmpty() || !nInpEditor.getText().containsOnly("0123456789"))
        return false;

    if (nHeadEditor.getText().isEmpty() || !nHeadEditor.getText().containsOnly("0123456789"))
        return false;

    if (nHidEditor.getText().isEmpty() || !nHidEditor.getText().containsOnly("0123456789"))
        return false;

    if (nLayersEditor.getText().isEmpty() || !nLayersEditor.getText().containsOnly("0123456789"))
        return false;

    return true;
}

void ModelConfigComponent::paint(juce::Graphics&) {

}

void ModelConfigComponent::resized()
{
    const int labelWidth = 120;
    const int editorHeight = 25;
    const int padding = 0;
    const int groupPadding = 15;

    const int modelGroupHeight = 4 * (editorHeight + padding) + 2 * groupPadding;

    modelGroup.setBounds(10, 10, getWidth() - 20, modelGroupHeight);

    const int labelX = 10;
    const int editorX = labelX + labelWidth + padding;
    const int editorWidth = modelGroup.getWidth() - labelWidth - 50;

    nInpLabel.setBounds(labelX, groupPadding, labelWidth, editorHeight);
    nInpEditor.setBounds(editorX, groupPadding, editorWidth, editorHeight);

    nHeadLabel.setBounds(labelX, groupPadding + editorHeight + padding, labelWidth, editorHeight);
    nHeadEditor.setBounds(editorX, groupPadding + editorHeight + padding, editorWidth, editorHeight);

    nHidLabel.setBounds(labelX, groupPadding + 2 * (editorHeight + padding), labelWidth, editorHeight);
    nHidEditor.setBounds(editorX, groupPadding + 2 * (editorHeight + padding), editorWidth, editorHeight);

    nLayersLabel.setBounds(labelX, groupPadding + 3 * (editorHeight + padding), labelWidth, editorHeight);
    nLayersEditor.setBounds(editorX, groupPadding + 3 * (editorHeight + padding), editorWidth, editorHeight);
}

int ModelConfigComponent::getRequiredHeight()
{
    const int editorHeight = 25;
    const int padding = 10;
    const int groupPadding = 15;

    return 5 * (editorHeight + padding) + 2 * groupPadding;
}