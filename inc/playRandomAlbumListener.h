#pragma once

#include "juce_events/juce_events.h"
#include "juce_core/juce_core.h"
#include "actionMessages.h"
#include "trackController.h"

class PlayRandomAlbumListener : public juce::ActionListener {
public:
    PlayRandomAlbumListener(TrackController& trackController)
        : trackController(trackController) {}

    void actionListenerCallback(const juce::String& message) {
        if (message == ActionMessages::playRandomAlbum) {
            trackController.playRandomAlbum();
        }
    }

private:
    TrackController& trackController;
};
