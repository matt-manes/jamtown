#pragma once

#include "juce_events/juce_events.h"
#include "browserComponent.h"
#include "playQueue.h"

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
