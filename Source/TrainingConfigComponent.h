/*
  ==============================================================================

    TrainingConfigComponent.h
    Created: 8 May 2024 11:50:22am
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <pybind11/embed.h>
#include <any>

#include "NumericInputFilter.h"

namespace py = pybind11;

class TrainingConfigComponent : public juce::Component
{
public:
    TrainingConfigComponent();
    ~TrainingConfigComponent();

    std::map<std::string, std::any> getInputValuesAsMap();
    bool areInputsValid();
    void paint(juce::Graphics&) override;
    void resized() override;

    int getRequiredHeight();

private:
    juce::GroupComponent trainingGroup{ "Training Configurations" };

    juce::Label epochsLabel{ "epochsLabel", "Epochs:" };
    juce::Label batchSizeLabel{ "batchSizeLabel", "Batch Size:" };
    juce::Label sequenceLengthLabel{ "sequenceLengthLabel", "Sequence Length:" };

    juce::TextEditor epochsEditor{ "epochsEditor" };
    juce::TextEditor batchSizeEditor{ "batchSizeEditor" };
    juce::TextEditor sequenceLengthEditor{ "sequenceLengthEditor" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrainingConfigComponent)
};
