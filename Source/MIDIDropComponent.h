/*
  ==============================================================================

    MIDIDropComponent.h
    Created: 3 May 2024 3:43:56pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MIDIDropComponent  : public juce::Component, public juce::FileDragAndDropTarget
{
public:
    MIDIDropComponent();
    ~MIDIDropComponent() override;

    bool isFileSet();
    juce::MidiFile& getMidiFile();

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

    void paint(juce::Graphics& g) override;

private:
    void storeMidiFile(const juce::File& file);

private:
    juce::MidiFile midiFile;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MIDIDropComponent)
};
