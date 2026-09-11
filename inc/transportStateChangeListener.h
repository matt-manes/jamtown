#pragma once

#include "trackController.h"
#include "transportController.h"
#include <juce_events/juce_events.h>

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
