#pragma once

#include <juce_events/juce_events.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "transportController.h"

class ScrubSlider : public juce::AnimatedAppComponent, public juce::ChangeListener {
public:
    ScrubSlider(TransportController* transportController);
    ~ScrubSlider() = default;

    void configureSlider();

    void configureLabels();

    void configureLayout();

    void configureElements();

    void resized() override;

    void update() override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    juce::Slider slider;

private:
    TransportController* transportController;
    juce::Label elapsedLabel;
    juce::Label remainingLabel;
    juce::FlexBox layoutBox;
    float labelWidth = 50.0f;
};
