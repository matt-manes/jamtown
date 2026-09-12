#pragma once

#include "juce_events/juce_events.h"
#include "browserComponent.h"
#include "playQueue.h"

/**
 * @brief Refreshes the play queue view after queue changes.
 *
 * Updates the browser component whenever the underlying play queue emits a change
 * notification so the UI reflects the latest queued tracks.
 */
class PlayQueueUpdatedListener : public juce::ChangeListener {
public:
    PlayQueueUpdatedListener(BrowserComponent& browserComponent, PlayQueue& playQueue)
        : browserComponent(browserComponent), playQueue(playQueue) {}

    void changeListenerCallback(juce::ChangeBroadcaster* /*source*/) {
        browserComponent.updatePlayQueueViewTrackList(playQueue.getTrackList());
    }

private:
    BrowserComponent& browserComponent;
    PlayQueue& playQueue;
};
