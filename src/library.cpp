#include "library.h"

std::vector<TrackInfo> Library::getArtistTracks(std::string artist) {
    auto albums = getAlbumsByArtist(artist);
    std::vector<TrackInfo> tracks;
    for (auto i = albums.begin(); i != albums.end(); ++i) {
        tracks.append_range(i->second);
    }
    return tracks;
}

void Library::addTracks(std::vector<TrackInfo> tracks) {
    for (auto track : tracks) {
        addTrack(track);
    }
}

void Library::removeTrack(TrackInfo track) {
    removeTrack(track.getTitle(), track.getAlbum(), track.getArtist());
}

void Library::removeTracks(std::vector<TrackInfo> tracks) {
    for (auto track : tracks)
        removeTrack(track);
}
