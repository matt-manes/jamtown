#pragma once

#include "juce_events/juce_events.h"
#include "juce_core/juce_core.h"
#include "playQueue.h"
#include "actionMessages.h"
#include "browserComponent.h"
#include "transportController.h"
#include "trackController.h"

class QueueTrackActionListener : public juce::ActionListener {
public:
    QueueTrackActionListener(PlayQueue& playQueue,
                             BrowserComponent& browserComponent,
                             TransportController& transportController,
                             TrackController& trackController)
        : playQueue(playQueue), browserComponent(browserComponent),
          transportController(transportController), trackController(trackController) {}

    void actionListenerCallback(const juce::String& message) override {
        if (message == ActionMessages::queueTrack) {
            playQueue.addTracks(browserComponent.getSelectedTracks());
            if (!transportController.hasActiveTrack())
                trackController.playNextTrack();
        }
    }

private:
    PlayQueue& playQueue;
    BrowserComponent& browserComponent;
    TransportController& transportController;
    TrackController& trackController;
};
