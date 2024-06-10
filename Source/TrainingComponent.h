/*
  ==============================================================================

    TrainingComponent.h
    Created: 3 May 2024 3:03:40pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include <filesystem>
#include <any>

#include "ListBoxComponent.h"
#include "ConsoleComponent.h"
#include "TrainingConfigComponent.h"
#include "ModelConfigComponent.h"

//==============================================================================
/*
*/
class TrainingComponent  : public juce::Component
{
public:
    TrainingComponent();
    ~TrainingComponent() override;

    std::map<std::string, std::any> prepareInput();
    void trainButtonClicked();
    void cancelButtonClicked();
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    ListBoxComponent listBoxComponent;
    TrainingConfigComponent trainingConfigComponent;
    ModelConfigComponent modelConfigComponent;

    ConsoleComponent consoleComponent;

    juce::TextButton trainButton{ "Generate" };
    juce::TextButton cancelButton{ "Cancel" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrainingComponent)
};
