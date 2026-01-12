#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <vector>
#include <string>
#include "trackInfo.h"
#include "browserView.h"
#include <utility>

/**
 * @brief BrowserView implementation for track library.
 *
 */
class LibraryView : public BrowserView {
public:
    LibraryView() { configureHeaders(); }
    ~LibraryView() = default;

    void configureHeaders() override;

    void cellDoubleClicked(int rowNumber,
                           int columnId,
                           const juce::MouseEvent& mouseEvent) override;

    void cellClicked(int rowNumber,
                     int columnId,
                     const juce::MouseEvent& mouseEvent) override;

    /**
     * @brief Get the track after the given one in the current sort order.
     *
     * @param currentTrack
     * @return TrackInfo
     */
    TrackInfo getNextTrack(TrackInfo currentTrack);

    void sortOrderChanged(int newSortColumnId, bool isForwards) override;

    void setTracklist(std::vector<TrackInfo> tracks) override;

    /**
     * @brief Set the currently playing track.
     *
     * @param track
     */
    void setCurrentlyPlayingTrack(TrackInfo track);

    void paintRowBackground(juce::Graphics& g,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;

    /**
     * @brief Get the most recently selected album to play.
     *
     * @return std::pair<std::string, std::string> First string is the album and the second is the artist.
     */
    std::pair<std::string, std::string> getAlbumToPlay();

    /**
     * @brief Get the most recently selected artist to play.
     *
     * @return std::string
     */
    std::string getArtistToPlay();

    /**
     * @brief Scroll the given track into view.
     *
     * @param track
     */
    void scrollTrackIntoView(TrackInfo track);

private:
    TrackInfo currentlyPlayingTrack;
    std::string albumToPlay;
    std::string artistToPlay;

    /**
     * @brief Show the context menu to the user.
     *
     * @param rowNumber
     */
    void showContextMenu(int rowNumber);
};
