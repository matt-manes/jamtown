#include "libraryView.h"
#include "actionMessages.h"
#include <stdexcept>
#include <algorithm>
#include <utility>
#include <string>

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
        menu.addItem(6, "Remove from library");
        menu.addItem(7, "Delete from harddrive");
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
                artistToPlay = getTrack(rowNumber).getArtist();
                sendActionMessage(ActionMessages::playAlbum);
                break;
            case 5:
                artistToPlay = getTrack(rowNumber).getArtist();
                sendActionMessage(ActionMessages::playArtist);
                break;
            case 6:
                sendActionMessage(ActionMessages::removeTracksFromLibrary);
                break;
            case 7:
                juce::AlertWindow::showOkCancelBox(
                    juce::MessageBoxIconType::WarningIcon,
                    "Confirm Delete",
                    "This will delete the track from your system permanently."
                    "Continue?",
                    "",
                    "",
                    nullptr,
                    juce::ModalCallbackFunction::create([this](int result) {
                        if (result)
                            sendActionMessage(ActionMessages::deleteTracksFromHarddrive);
                    }));
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
    TrackSorter::sort(tracklist,
                      table.getHeader().getColumnName(newSortColumnId).toStdString(),
                      isForwards);
    table.updateContent();
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

std::pair<std::string, std::string> LibraryView::getAlbumToPlay() {
    return std::pair<std::string, std::string>{albumToPlay, artistToPlay};
}
std::string LibraryView::getArtistToPlay() { return artistToPlay; }

void LibraryView::scrollTrackIntoView(TrackInfo track) {
    int rowNum = 0;
    for (int i = 0; i < tracklist.size(); ++i) {
        if (tracklist[i] == track) {
            rowNum = i;
            break;
        }
    }
    table.scrollToEnsureRowIsOnscreen(rowNum);
}
