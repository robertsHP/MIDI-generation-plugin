#include "MainComponent.h"

#include "PluginProcessor.h"

//==============================================================================
MainComponent::MainComponent()
{
    addAndMakeVisible(header);

    addAndMakeVisible(generatingComponent);
    addAndMakeVisible(trainingComponent);

    header.onSwitchComponent = [this](int which) { switchComponent(which); };

    switchComponent(HeaderComponent::View::GENERATE); // Default to Component A

    //addAndMakeVisible(midiVisualizerComponent);
    setSize (700, 600);
}

MainComponent::~MainComponent()
{

}

void MainComponent::switchComponent(int which)
{
    if (!PluginProcessor::pythonBridge.taskRunning) {
        generatingComponent.setVisible(which == HeaderComponent::View::GENERATE);
        trainingComponent.setVisible(which == HeaderComponent::View::TRAIN);
    }
    else {
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::InfoIcon, "Error", "Cannot perform action because a task is currently running!");
    }
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    //g.setFont (juce::Font (16.0f));
    //g.setColour (juce::Colours::white);
    //g.drawText ("Hello World!", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    auto area = getLocalBounds();
    header.setBounds(area.removeFromTop(20));

    generatingComponent.setBounds(area);
    trainingComponent.setBounds(area);

    //midiVisualizerComponent.setBounds(getLocalBounds());
}
