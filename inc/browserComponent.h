#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_events/juce_events.h>
#include <vector>
#include <string>
#include <memory>
#include "inMemLibrary.h"
#include "trackInfo.h"
#include "playQueue.h"
#include "browserView.h"
#include "library.h"
#include "actionMessages.h"
#include "libraryView.h"
#include "playQueueView.h"

enum View { LIBRARY, PLAYQUEUE };

/**
 * @brief Browser component that can be set to display various `BrowserView` objects.
 *
 */
class BrowserComponent : public juce::Component,
                         public juce::ActionBroadcaster,
                         public juce::ActionListener {
public:
    BrowserComponent();

    ~BrowserComponent() = default;

    void resized() override {
        libraryView->setBounds(0, 0, getWidth(), getHeight());
        playQueueView->setBounds(0, 0, getWidth(), getHeight());
    };

    /**
     * @brief Get tracks selected in current view.
     *
     * @return std::vector<TrackInfo>
     */
    std::vector<TrackInfo> getSelectedTracks() {
        return currentView->getSelectedTracks();
    }

    void actionListenerCallback(const juce::String& message) override {
        if (message == ActionMessages::libraryUpdated) {
            libraryView->setTracklist(library->getAllTracks());
        } else if (message == ActionMessages::playQueueUpdated) {
            playQueueView->setTracklist(playQueue->getTrackList());
        } else  // propogate action messages sent from view objects
            sendActionMessage(message);
    }

    /**
     * @brief Set the library instance to use.
     *
     * @param newLibrary
     */
    void setLibrary(Library* newLibrary) {
        library = newLibrary;
        libraryView->setTracklist(library->getAllTracks());
    }

    /**
     * @brief Set the PlayQueue instance to be use.
     *
     * @param newQueue
     */
    void setPlayQueue(PlayQueue* newQueue) {
        playQueue = newQueue;
        playQueueView->setTracklist(playQueue->getTrackList());
    }

    /**
     * @brief Set the view to display.
     *
     * @param view
     */
    void setView(View view);

    TrackInfo getNextLibraryTrack(TrackInfo currentTrack);

private:
    Library* library;
    PlayQueue* playQueue;
    TrackInfo selectedTrack;
    int selectedTrackIndex = -1;
    std::unique_ptr<LibraryView> libraryView;
    std::unique_ptr<PlayQueueView> playQueueView;
    BrowserView* currentView = nullptr;
};
