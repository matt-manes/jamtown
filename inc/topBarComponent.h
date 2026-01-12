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

    /**
     * @brief Get the files found during the last scan.
     *
     * @return juce::Array<juce::File>
     */
    juce::Array<juce::File> getTrackAdderFiles();

    /**
     * @brief Set the wildcard to use when searching for files.
     *
     * @param wildcard A ';' seperated list of file extension wildcards.
     */
    void setTrackAdderWildcard(juce::String wildcard);

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

    /**
     * @brief Get the width components should be displayed with.
     *
     * @return int
     */
    int getElementWidth();
};
