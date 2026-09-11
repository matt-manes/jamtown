#pragma once

#include "juce_events/juce_events.h"
#include "juce_core/juce_core.h"
#include "actionMessages.h"
#include "library.h"
#include "trackController.h"
#include "browserComponent.h"

class PlayArtistListener : public juce::ActionListener {
public:
    PlayArtistListener(Library& library,
                       TrackController& trackController,
                       BrowserComponent& browserComponent)
        : library(library), trackController(trackController),
          browserComponent(browserComponent) {}

    void actionListenerCallback(const juce::String& message) override {
        if (message == ActionMessages::playArtist) {
            trackController.overwritePlayQueue(
                library.getArtistTracks(browserComponent.getArtistToPlay()), "Album");
        }
    }

private:
    Library& library;
    TrackController& trackController;
    BrowserComponent& browserComponent;
};
