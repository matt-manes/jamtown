#pragma once

#include "juce_events/juce_events.h"
#include "juce_core/juce_core.h"
#include "actionMessages.h"
#include "library.h"
#include "trackController.h"
#include "browserComponent.h"

/**
 * @brief Starts playback of the selected album.
 *
 * Responds to the album-play action by replacing the current queue with the tracks
 * belonging to the album selected in the browser.
 */
class PlayAlbumListener : public juce::ActionListener {
public:
    PlayAlbumListener(Library& library,
                      TrackController& trackController,
                      BrowserComponent& browserComponent)
        : library(library), trackController(trackController),
          browserComponent(browserComponent) {}

    void actionListenerCallback(const juce::String& message) override {
        if (message == ActionMessages::playAlbum) {
            auto selection = browserComponent.getAlbumToPlay();
            trackController.overwritePlayQueue(
                library.getAlbumTracks(selection.first, selection.second), "Title");
        }
    }

private:
    Library& library;
    TrackController& trackController;
    BrowserComponent& browserComponent;
};
