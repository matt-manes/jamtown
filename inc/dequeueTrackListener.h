#pragma once

#include "juce_events/juce_events.h"
#include "juce_core/juce_core.h"
#include "playQueue.h"
#include "actionMessages.h"
#include "browserComponent.h"

class DequeueTrackListener : public juce::ActionListener {
public:
    DequeueTrackListener(PlayQueue& playQueue, BrowserComponent& browserComponent)
        : playQueue(playQueue), browserComponent(browserComponent) {}

    void actionListenerCallback(const juce::String& message) override {
        if (message == ActionMessages::removeSelectedFromPlayQueue) {
            playQueue.remove(browserComponent.getSelectedTracks());
        }
    }

private:
    PlayQueue& playQueue;
    BrowserComponent& browserComponent;
};
