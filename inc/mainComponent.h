#pragma once

#include "transportComponent.h"
#include "addTracksComponent.h"
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>
#include <memory>
#include "playQueue.h"
#include "fileProcessor.h"
#include "inMemLibrary.h"
#include "libraryComponent.h"

class MainComponent : public juce::Component,
                      public juce::ChangeListener,
                      public juce::ActionListener {
public:
    MainComponent();

    void paint(juce::Graphics& g) override;

    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster* source);

    void actionListenerCallback(const juce::String& message);

    void playTrack(TrackInfo track);

private:
    TransportComponent transport;
    AddTracksComponent trackAdder;
    FileProcessor fileProcessor;
    int transportPadding = 10;
    LibraryComponent library;
    PlayQueue playQueue;

    void configureTransport();
    void configureTrackAdder();
    void configureLibrary();
    void configureElements();
    void resizeTrackAdder();
    void resizeLibrary();
    void resizeTransport();
    void handleTracksAdded();
    void handleTransportChange();
    void handlePlayMessage();
    void handleQueueMessage();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
