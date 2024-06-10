/*
  ==============================================================================

    NumericInputFilter.cpp
    Created: 10 May 2024 5:48:48pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "NumericInputFilter.h"

//==============================================================================
NumericInputFilter::NumericInputFilter(bool allowDecimal)
    : decimalAllowed(allowDecimal)
{
}

juce::String NumericInputFilter::filterNewText(juce::TextEditor& editor, const juce::String& newInput)
{
    juce::String currentText = editor.getText();
    juce::String combinedText = currentText + newInput;

    std::regex pattern(decimalAllowed ? R"(^\d*\.?\d*$)" : R"(^\d*$)");

    if (std::regex_match(combinedText.toStdString(), pattern))
        return newInput;

    return {};
}