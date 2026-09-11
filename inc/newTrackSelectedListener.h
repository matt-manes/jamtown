#pragma once

#include "juce_events/juce_events.h"
#include "browserComponent.h"
#include "transportController.h"

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
