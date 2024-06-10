/*
  ==============================================================================

    NumericInputFilter.h
    Created: 10 May 2024 5:48:48pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <regex>

//==============================================================================
/*
*/

class NumericInputFilter : public juce::TextEditor::InputFilter
{
public:
    NumericInputFilter(bool allowDecimal = false);

    juce::String filterNewText(juce::TextEditor& editor, const juce::String& newInput) override;

private:
    bool decimalAllowed;
};

