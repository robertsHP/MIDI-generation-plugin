/*
  ==============================================================================

    MIDIDragComponent.cpp
    Created: 11 May 2024 8:40:30pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MIDIDragComponent.h"

//==============================================================================

MIDIDragComponent::MIDIDragComponent()
{
}

MIDIDragComponent::~MIDIDragComponent()
{
}

bool MIDIDragComponent::isFileSet() {
    juce::File midiFilePath(filePath);
    return midiFilePath.existsAsFile() && midiFile.getNumTracks() > 0;
}

void MIDIDragComponent::setMidiFile(juce::String& path, juce::MidiFile& file)
{
    filePath = path;
    midiFile = file;

    if (juce::MessageManager::getInstance()->isThisTheMessageThread())
    {
        repaint();
    }
    else
    {
        juce::MessageManager::callAsync([this]() { repaint(); });
    }
}
void MIDIDragComponent::mouseDrag(const juce::MouseEvent& event)
{
    if (!isFileSet()) return;

    juce::StringArray files;

    files.add(filePath);
    performExternalDragDropOfFiles(files, false);
}

void MIDIDragComponent::resized()
{
    
}

void MIDIDragComponent::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::black);

    if (isFileSet()) {
        g.fillAll(juce::Colours::green);  // clear the background
        g.drawText("Output generated!", getLocalBounds(), juce::Justification::centred, true);
    } else {
        g.fillAll(juce::Colours::red);  // clear the background
        g.drawText("No output generated!", getLocalBounds(), juce::Justification::centred, true);
    }
}