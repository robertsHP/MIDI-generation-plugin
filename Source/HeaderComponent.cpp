/*
  ==============================================================================

    HeaderComponent.cpp
    Created: 3 May 2024 2:58:56pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "HeaderComponent.h"

//==============================================================================
HeaderComponent::HeaderComponent()
{
    selectedView = View::GENERATE;

    addAndMakeVisible(generationViewButton);
    addAndMakeVisible(trainingViewButton);

    updateButtonStyles();

    generationViewButton.onClick = [this] { buttonClicked(View::GENERATE); };
    trainingViewButton.onClick = [this] { buttonClicked(View::TRAIN); };
}

HeaderComponent::~HeaderComponent()
{

}


void HeaderComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
}

void HeaderComponent::resized()
{
    auto area = getLocalBounds();
    generationViewButton.setBounds(area.removeFromLeft(getWidth() / 2));
    trainingViewButton.setBounds(area);
}

void HeaderComponent::buttonClicked(View selView)
{
    selectedView = selView;
    if (onSwitchComponent) 
        onSwitchComponent(selectedView);
    updateButtonStyles();
}

void HeaderComponent::updateButtonStyles()
{
	juce::Colour genButtonColor = juce::Colours::grey;
    juce::Colour trainingButtonColor = juce::Colours::grey;

    switch (selectedView) {
        case View::GENERATE:
            genButtonColor = juce::Colours::lightblue;
            break;
        case View::TRAIN:
            trainingButtonColor = juce::Colours::lightblue;
            break;
    }

    generationViewButton.setColour(juce::TextButton::buttonColourId, genButtonColor);
    trainingViewButton.setColour(juce::TextButton::buttonColourId, trainingButtonColor);
}
