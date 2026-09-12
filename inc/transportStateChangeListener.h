#pragma once

#include "trackController.h"
#include "transportController.h"
#include <juce_events/juce_events.h>

/**
 * @brief Advances playback when the transport has no active track.
 *
 * Watches transport changes and automatically moves to the next track if the
 * current transport state indicates there is no track playing.
 */
class TransportStateChangeListener : public juce::ChangeListener {
public:
    TransportStateChangeListener(TrackController& trackController,
                                 TransportController& transportController)
        : trackController(trackController), transportController(transportController) {}

    void changeListenerCallback(juce::ChangeBroadcaster* /*source*/) override {
        if (!transportController.hasActiveTrack())
            trackController.playNextTrack();
    }

private:
    TrackController& trackController;
    TransportController& transportController;
};
