/*
  ==============================================================================

    ConsoleComponent.h
    Created: 8 May 2024 11:40:25am
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ConsoleComponent  : public juce::Component, private juce::TextEditor::Listener
{
public:
    ConsoleComponent();
    ~ConsoleComponent() override;

    void appendText(const std::string& text);
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::TextEditor console;
    std::mutex consoleMutex;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ConsoleComponent)
};
