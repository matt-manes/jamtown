#pragma once

#include "juce_events/juce_events.h"
#include "juce_core/juce_core.h"
#include "playQueue.h"
#include "actionMessages.h"
#include "browserComponent.h"
#include "transportController.h"
#include "trackController.h"

/**
 * @brief Queues selected tracks and starts playback when needed.
 *
 * Handles the queue action by adding the user's selections to the play queue and
 * beginning playback if no track is currently active.
 */
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
