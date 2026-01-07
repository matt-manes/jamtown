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

    TrackInfo getNextTrack(TrackInfo currentTrack);

    void sortOrderChanged(int newSortColumnId, bool isForwards) override;

    void setTracklist(std::vector<TrackInfo> tracks) override;

    void setCurrentlyPlayingTrack(TrackInfo track);

    void paintRowBackground(juce::Graphics& g,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;

    std::pair<std::string, std::string> getAlbumToPlay();

    std::string getArtistToPlay();

    void scrollTrackIntoView(TrackInfo track);

private:
    TrackInfo currentlyPlayingTrack;
    std::string albumToPlay;
    std::string artistToPlay;

    void showContextMenu(int rowNumber);
};
