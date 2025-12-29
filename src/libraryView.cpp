#include "libraryView.h"
#include "actionMessages.h"
#include <stdexcept>
#include <algorithm>

void LibraryView::configureHeaders() {
    addAndMakeVisible(table);
    table.getHeader().setStretchToFitActive(true);
    table.setColour(juce::ListBox::backgroundColourId, juce::Colours::black);
    table.setColour(juce::ListBox::textColourId, juce::Colours::black);
    table.getHeader().setColour(juce::TableHeaderComponent::backgroundColourId,
                                juce::Colours::aquamarine);
    table.setHeaderHeight(24);
    table.getHeader().addColumn(
        "Title", 1, 100, 10, -1, juce::TableHeaderComponent::defaultFlags);
    table.getHeader().addColumn(
        "Album", 2, 100, 10, -1, juce::TableHeaderComponent::defaultFlags);
    table.getHeader().addColumn(
        "Artist", 3, 100, 10, -1, juce::TableHeaderComponent::defaultFlags);
    table.getHeader().addColumn(
        "Length", 4, 100, 10, -1, juce::TableHeaderComponent::defaultFlags);
    table.setMultipleSelectionEnabled(true);
    // Default to Artist
    table.getHeader().setSortColumnId(3, true);
}

void LibraryView::cellDoubleClicked(int rowNumber,
                                    int /*columnId*/,
                                    const juce::MouseEvent& mouseEvent) {
    if (mouseEvent.mods.isLeftButtonDown() && rowNumber < getNumRows()) {
        sendActionMessage(ActionMessages::loadSelectedTracks);
    }
}

void LibraryView::cellClicked(int rowNumber,
                              int /*columnId*/,
                              const juce::MouseEvent& mouseEvent) {
    if (mouseEvent.mods.isRightButtonDown() && rowNumber < getNumRows()) {
        juce::PopupMenu menu;
        menu.addItem(1, "Play");
        menu.addItem(2, "Add to queue");
        menu.addItem(3, "Add to playlist", false);
        menu.addItem(4, "Play album");
        menu.addItem(5, "Play artist");
        menu.showMenuAsync(juce::PopupMenu::Options(), [this, rowNumber](int result) {
            switch (result) {
            case 1:
                sendActionMessage(ActionMessages::loadSelectedTracks);
                break;
            case 2:
                sendActionMessage(ActionMessages::queueTrack);
                break;
            case 4:
                albumToPlay = getTrack(rowNumber).getAlbum();
                sendActionMessage(ActionMessages::playAlbum);
                break;
            case 5:
                artistToPlay = getTrack(rowNumber).getArtist();
                sendActionMessage(ActionMessages::playArtist);
                break;
            // TODO implement other options
            default:
                break;
            }
        });
    }
}

TrackInfo LibraryView::getNextTrack(TrackInfo currentTrack) {
    // Doing this here so that the next track depends on
    // the current sort order of the table.
    // TODO improve big O by keeping like a trackname to index mapping or w/e
    if (tracklist.back().getPath() == currentTrack.getPath())
        return tracklist[0];
    for (auto i = tracklist.cbegin(); i != tracklist.cend(); ++i) {
        if (*i == currentTrack)
            return *(i + 1);
    }
    throw std::invalid_argument(std::format("Could not find {} in LibraryView tracklist.",
                                            currentTrack.toString()));
}

void LibraryView::sortOrderChanged(int newSortColumnId, bool isForwards) {
    TrackInfoComparator* sorter = TrackComparatorFactory::getTrackComparator(
        table.getHeader().getColumnName(newSortColumnId).toStdString(), isForwards);
    std::sort(
        tracklist.begin(), tracklist.end(), [sorter](TrackInfo first, TrackInfo second) {
            return sorter->goesBefore(first, second);
        });
    table.updateContent();
    delete sorter;
}

void LibraryView::setTracklist(std::vector<TrackInfo> tracks) {
    BrowserView::setTracklist(tracks);
    table.getHeader().reSortTable();
}

void LibraryView::setCurrentlyPlayingTrack(TrackInfo track) {
    currentlyPlayingTrack = track;
    repaint();
}

void LibraryView::paintRowBackground(
    juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
    TrackInfo track = getTrack(rowNumber);
    if (track.getPath() == currentlyPlayingTrack.getPath()) {
        g.fillAll(juce::Colours::turquoise);
        return;
    }
    BrowserView::paintRowBackground(g, rowNumber, width, height, rowIsSelected);
}

std::string LibraryView::getAlbumToPlay() { return albumToPlay; }
std::string LibraryView::getArtistToPlay() { return artistToPlay; }
