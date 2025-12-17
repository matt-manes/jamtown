#pragma once

#include "transportComponent.h"
#include "addTracksComponent.h"
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>
#include <memory>
#include "fileProcessor.h"
#include "inMemLibrary.h"

class MainComponent : public juce::Component, public juce::ChangeListener {
 public:
    MainComponent();

    void paint(juce::Graphics& g) override;

    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster* source);

 private:
    TransportComponent transport;
    AddTracksComponent trackAdder;
    FileProcessor fileProcessor;
    int transportPadding = 10;
    InMemLibrary library;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
