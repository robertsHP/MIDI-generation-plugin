/*
  ==============================================================================

    MIDIDropComponent.cpp
    Created: 3 May 2024 3:43:56pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MIDIDropComponent.h"

//==============================================================================
MIDIDropComponent::MIDIDropComponent()
{
}

MIDIDropComponent::~MIDIDropComponent()
{
}

bool MIDIDropComponent::isFileSet() {
    return midiFile.getNumTracks() > 0;
}

juce::MidiFile& MIDIDropComponent::getMidiFile()
{
    return midiFile;
}

bool MIDIDropComponent::isInterestedInFileDrag(const juce::StringArray& files) {
    for (auto file : files) {
        bool lowerCaseCheck = (file.endsWith("mid") || file.endsWith("midi"));
        bool upperCaseCheck = (file.endsWith("MID") || file.endsWith("MIDI"));

        if (lowerCaseCheck || upperCaseCheck)
            return true;
    }
    return false;
}

void MIDIDropComponent::filesDropped(const juce::StringArray& files, int x, int y) {
    for (auto& file : files) {
        juce::File midiFile(file);

        bool lowerCaseCheck = (midiFile.hasFileExtension("mid") || midiFile.hasFileExtension("midi"));
        bool upperCaseCheck = (midiFile.hasFileExtension("MID") || midiFile.hasFileExtension("MIDI"));

        if (midiFile.existsAsFile() && lowerCaseCheck || upperCaseCheck) {
            juce::MidiFile midi;
            juce::FileInputStream inputStream(midiFile);
            if (inputStream.openedOk() && midi.readFrom(inputStream)) {
                this->midiFile = midi;
                repaint();
                break;
            }
        }
    }
}

void MIDIDropComponent::paint(juce::Graphics& g) {
    g.setColour(juce::Colours::black);

    if (isFileSet()) {
        g.fillAll(juce::Colours::green);
        g.drawText("MIDI file assigned!", getLocalBounds(), juce::Justification::centred, true);
    }
    else {
        g.fillAll(juce::Colours::red);
        g.drawText("Drag and drop the input MIDI file here", getLocalBounds(), juce::Justification::centred, true);
    }
}

void MIDIDropComponent::storeMidiFile(const juce::File& file) {
    juce::FileInputStream inputStream(file);
    if (inputStream.openedOk()) {
        midiFile.readFrom(inputStream);
    }
}
