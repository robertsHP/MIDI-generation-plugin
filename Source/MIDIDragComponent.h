/*
  ==============================================================================

    MIDIDragComponent.h
    Created: 11 May 2024 8:40:30pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MIDIDragComponent : public juce::Component, public juce::DragAndDropContainer
{
    public:
        MIDIDragComponent();
        ~MIDIDragComponent();

        bool isFileSet();
        void setMidiFile(juce::String& path, juce::MidiFile& file);

        void mouseDrag(const juce::MouseEvent& event) override;

        void resized() override;
        void paint(juce::Graphics& g) override;

    private:
        juce::MidiFile midiFile;
        juce::String filePath;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MIDIDragComponent)
};
