/*
  ==============================================================================

    GeneratingComponent.h
    Created: 3 May 2024 3:03:51pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include <filesystem>
#include <any>

#include "ListBoxComponent.h"
#include "MIDIDropComponent.h"
#include "MIDIDragComponent.h"
#include "ConsoleComponent.h"

//==============================================================================
/*
*/

class GeneratingComponent  : public juce::Component
{
public:
    GeneratingComponent();
    ~GeneratingComponent() override;

private:
    std::map<std::string, std::any> prepareInput();
public:
    void generateButtonClicked();
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    //MIDIVisualizerComponent midiVisualizerComponent{ juce::Colours::green };
    MIDIDropComponent midiDropComponent;
    juce::TextButton generateButton;
    MIDIDragComponent midiDragComponent;
    ListBoxComponent listBoxComponent;

    ConsoleComponent consoleComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GeneratingComponent)
};
