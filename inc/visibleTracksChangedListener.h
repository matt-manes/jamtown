#pragma once

#include "juce_events/juce_events.h"
#include "browserComponent.h"

class VisibleTracksChangedListener : public juce::ChangeListener {
public:
    VisibleTracksChangedListener(BrowserComponent& browserComponent)
        : browserComponent(browserComponent) {}

    void changeListenerCallback(juce::ChangeBroadcaster* /*source*/) override {
        browserComponent.updateVisibleTrackDataComponent();
    }

private:
    BrowserComponent& browserComponent;
};
