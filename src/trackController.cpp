#include "trackController.h"
#include <vector>
#include <string>

TrackInfo TrackController::getRandomTrackToPlay() {
    if (library.getTrackCount() <= 1)
        return transportController.getCurrentTrack();
    auto track = library.getRandomTrack();
    while (track == transportController.getCurrentTrack())
        track = library.getRandomTrack();
    return track;
}

std::vector<TrackInfo> TrackController::getRandomAlbumToPlay() {
    auto tracks = library.getRandomAlbumTracks();
    if (library.getAllAlbumTitles().size() <= 1)
        return tracks;
    while (tracks[0].getAlbum() == transportController.getCurrentTrack().getAlbum() &&
           tracks[0].getArtist() == transportController.getCurrentTrack().getArtist())
        tracks = library.getRandomAlbumTracks();
    return tracks;
}

void TrackController::playNextTrack() {
    if (!playQueue.empty()) {
        transportController.playTrack(playQueue.getNextTrack());
        sendChangeMessage();
        return;
    }
    auto shuffleMode = transportComponent.getCurrentShuffleMode();
    if (shuffleMode == ShuffleMode::OFF)
        transportController.playTrack(
            browser.getNextLibraryViewTrack(transportController.getCurrentTrack()));
    else if (shuffleMode == ShuffleMode::TRACK)
        transportController.playTrack(getRandomTrackToPlay());
    else if (shuffleMode == ShuffleMode::ALBUM)
        overwritePlayQueue(getRandomAlbumToPlay(), "Title");
    sendChangeMessage();
}

void TrackController::overwritePlayQueue(std::vector<TrackInfo> tracks,
                                         std::string sortAttribute) {
    playQueue.clear();
    if (!sortAttribute.empty()) {
        TrackSorter::sort(tracks, sortAttribute, true);
    }
    playQueue.addTracks(tracks);
    playNextTrack();
}

void TrackController::playRandomAlbum() {
    overwritePlayQueue(getRandomAlbumToPlay(), "Title");
}
