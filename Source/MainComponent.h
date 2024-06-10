#pragma once

#include <JuceHeader.h>

#include "HeaderComponent.h"

#include "GeneratingComponent.h"
#include "TrainingComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    void switchComponent(int which);

private:
    //==============================================================================
    // Your private member variables go here...
    HeaderComponent header;

    GeneratingComponent generatingComponent;
    TrainingComponent trainingComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
