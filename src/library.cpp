#include "library.h"

std::vector<TrackInfo> Library::getArtistTracks(std::string artist) {
    auto albums = getAlbums(artist);
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
