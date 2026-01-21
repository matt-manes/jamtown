#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_events/juce_events.h>
#include <vector>
#include <string>
#include <memory>
#include "trackInfo.h"
#include "playQueue.h"
#include "browserView.h"
#include "library.h"
#include "libraryView.h"
#include "playQueueView.h"
#include <utility>

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

    void resized() override;

    /**
     * @brief Get tracks selected in current view.
     *
     * @return std::vector<TrackInfo>
     */
    std::vector<TrackInfo> getSelectedTracks();

    void actionListenerCallback(const juce::String& message) override;

    /**
     * @brief Set the library instance to use.
     *
     * @param newLibrary
     */
    void setLibrary(Library* newLibrary);

    /**
     * @brief Set the PlayQueue instance to be use.
     *
     * @param newQueue
     */
    void setPlayQueue(PlayQueue* newQueue);

    /**
     * @brief Set the view to display.
     *
     * @param view
     */
    void setView(View view);

    /**
     * @brief Get the track after the given one in the current sort order.
     *
     * @param currentTrack
     * @return TrackInfo
     */
    TrackInfo getNextLibraryViewTrack(TrackInfo currentTrack);

    /**
     * @brief Set what the currently playing track is.
     *
     * @param track
     */
    void setCurrentlyPlayingTrack(TrackInfo track);

    /**
     * @brief Get the most recently selected album.
     *
     * @return std::pair<std::string, std::string> The first is the album title and the second is the artist name.
     */
    std::pair<std::string, std::string> getAlbumToPlay();

    /**
     * @brief Get the most recently selected artist.
     *
     * @return std::string
     */
    std::string getArtistToPlay();

    void updateLibraryViewTrackList(std::vector<TrackInfo> tracks);

private:
    Library* library;
    PlayQueue* playQueue;
    TrackInfo selectedTrack;
    int selectedTrackIndex = -1;
    std::unique_ptr<LibraryView> libraryView;
    std::unique_ptr<PlayQueueView> playQueueView;
    BrowserView* currentView = nullptr;
};
