#pragma once

#include "addTracksComponent.h"
#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>

class TopBarComponent : public juce::Component,
                        public juce::ChangeListener,
                        public juce::ActionBroadcaster {
public:
    TopBarComponent();
    ~TopBarComponent() = default;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void resized() override;

    juce::Array<juce::File> getTrackAdderFiles() { return trackAdder.getResults(); }

    void setTrackAdderWildcard(juce::String wildcard) {
        trackAdder.setScannerWildcard(wildcard);
    }

private:
    AddTracksComponent trackAdder;
    juce::TextButton viewLibrary;
    juce::TextButton viewPlayQueue;
    int numElements = 3;
    int buttonSpacing = 5;

    void viewLibraryClicked();
    void viewPlayQueueClicked();

    void configureElements();
    void configureTrackAdder();
    void configureViewLibrary();
    void configureViewPlayQueue();
    void applyButtonStyle(juce::TextButton& button);

    int getElementWidth();
};
