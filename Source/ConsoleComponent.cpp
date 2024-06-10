/*
  ==============================================================================

    ConsoleComponent.cpp
    Created: 8 May 2024 11:40:25am
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ConsoleComponent.h"

//==============================================================================
ConsoleComponent::ConsoleComponent()
{
    addAndMakeVisible(console);
    console.setMultiLine(true);
    console.setReadOnly(true);
    console.setReturnKeyStartsNewLine(false);
    console.addListener(this);
    console.setCaretVisible(false);
    console.setScrollbarsShown(true);
    console.setFont(juce::Font(14.0f, juce::Font::plain));
}

ConsoleComponent::~ConsoleComponent()
{
}

void ConsoleComponent::appendText(const std::string& text) {
    std::lock_guard<std::mutex> lock(consoleMutex);

    juce::MessageManager::callAsync([this, text] {
        console.moveCaretToEnd();
        console.insertTextAtCaret(text + "\n");
    });
}

void ConsoleComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void ConsoleComponent::resized()
{
    console.setBounds(getLocalBounds());
}
