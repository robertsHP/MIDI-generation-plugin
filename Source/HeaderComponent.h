/*
  ==============================================================================

    HeaderComponent.h
    Created: 3 May 2024 2:58:56pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class HeaderComponent  : public juce::Component
{
public:
    enum View
	{
		GENERATE,
		TRAIN
	};

    HeaderComponent();
    ~HeaderComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    std::function<void(int)> onSwitchComponent;

private:
    juce::TextButton generationViewButton { "Generate" };
    juce::TextButton trainingViewButton{ "Train" };

    View selectedView;

    void buttonClicked(View selView);
    void updateButtonStyles();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HeaderComponent)
};
