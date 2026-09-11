#pragma once

#include "juce_events/juce_events.h"
#include "juce_core/juce_core.h"
#include "browserComponent.h"
#include "transportController.h"
#include "actionMessages.h"

class LoadSelectedTracksListener : public juce::ActionListener {
public:
    LoadSelectedTracksListener(BrowserComponent& browserComponent,
                               TransportController& transportController,
                               PlayQueue& playQueue)
        : browserComponent(browserComponent), transportController(transportController),
          playQueue(playQueue) {}

    void actionListenerCallback(const juce::String& message) override {
        if (message == ActionMessages::loadSelectedTracks) {
            auto tracks = browserComponent.getSelectedTracks();
            transportController.playTrack(tracks[0]);
            // If multiple tracks were selected, add the rest to the queue
            if (tracks.size() > 1) {
                for (auto i = (tracks.begin() + 1); i != tracks.end(); ++i) {
                    playQueue.addTrack(*i);
                }
            }
        }
    }

private:
    BrowserComponent& browserComponent;
    TransportController& transportController;
    PlayQueue& playQueue;
};
