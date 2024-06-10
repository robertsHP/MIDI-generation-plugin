/*
  ==============================================================================

    ModelConfigComponent.h
    Created: 8 May 2024 11:50:04am
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <any>

#include "NumericInputFilter.h"

class ModelConfigComponent : public juce::Component
{
public:
    ModelConfigComponent();
    ~ModelConfigComponent();

    std::map<std::string, std::any> getInputValuesAsMap();
    bool areInputsValid();
    void paint(juce::Graphics&) override;
    void resized() override;

    int getRequiredHeight();

private:
    juce::GroupComponent modelGroup{ "Model Configurations" };

    juce::Label nTokensLabel{ "nTokensLabel", "Number of Tokens:" };
    juce::Label nInpLabel{ "nInpLabel", "Input Size:" };
    juce::Label nHeadLabel{ "nHeadLabel", "Number of Heads:" };
    juce::Label nHidLabel{ "nHidLabel", "Hidden Size:" };
    juce::Label nLayersLabel{ "nLayersLabel", "Number of Layers:" };

public:
    juce::TextEditor nTokensEditor{ "nTokensEditor" };
    juce::TextEditor nInpEditor{ "nInpEditor" };
    juce::TextEditor nHeadEditor{ "nHeadEditor" };
    juce::TextEditor nHidEditor{ "nHidEditor" };
    juce::TextEditor nLayersEditor{ "nLayersEditor" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModelConfigComponent)
};
