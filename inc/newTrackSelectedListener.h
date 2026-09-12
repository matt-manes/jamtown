#pragma once

#include "juce_events/juce_events.h"
#include "browserComponent.h"
#include "transportController.h"

/**
 * @brief Keeps the browser UI in sync with the current playback selection.
 *
 * Tracks the active transport selection and updates the component state whenever a
 * new track becomes the currently playing item.
 */
class NewTrackSelectedListener : public juce::ChangeListener {
public:
    NewTrackSelectedListener(BrowserComponent& browserComponent,
                             TransportController& transportController)
        : browserComponent(browserComponent), transportController(transportController) {}

    void changeListenerCallback(juce::ChangeBroadcaster* /*source*/) override {
        browserComponent.setCurrentlyPlayingTrack(transportController.getCurrentTrack());
    }

private:
    BrowserComponent& browserComponent;
    TransportController& transportController;
};
