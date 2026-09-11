#pragma once

#include "juce_events/juce_events.h"
#include "juce_core/juce_core.h"
#include "actionMessages.h"
#include "trackController.h"
#include "transportController.h"

class TransportCommandListener : public juce::ActionListener {
public:
    TransportCommandListener(TrackController& trackController,
                             TransportController& transportController)
        : trackController(trackController), transportController(transportController) {}

    void actionListenerCallback(const juce::String& message) override {
        if (message == ActionMessages::nextTrack)
            trackController.playNextTrack();
        else if (message == ActionMessages::restartTrack)
            transportController.setPosition(0.0);
        else if (message == ActionMessages::stopTrack)
            transportController.stop();
        else if (message == ActionMessages::playTrack)
            transportController.start();
        else if (message == ActionMessages::pauseTrack)
            transportController.pause();
    }

private:
    TrackController& trackController;
    TransportController& transportController;
};
