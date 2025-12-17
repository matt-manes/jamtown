#include "inMemLibrary.h"

void InMemLibrary::addTrack(TrackInfo track) {
    titleToTrackMap[track.getTitle()] = track;
    albumToTrackTitleMap[track.getAlbum()].insert(track.getTitle());
    artistToAlbumMap[track.getArtist()].insert(track.getAlbum());
}

std::unordered_map<std::string, std::vector<TrackInfo>> InMemLibrary::getAlbums(
    std::string artist) {
    auto albumTitles = artistToAlbumMap[artist];
    std::unordered_map<std::string, std::vector<TrackInfo>> albums;
    for (auto album : albumTitles) {
        albums[album] = getAlbum(album);
    }
    return albums;
}

std::vector<TrackInfo> InMemLibrary::getAlbum(std::string album) {
    auto titles = albumToTrackTitleMap[album];
    std::vector<TrackInfo> tracks;
    for (auto title : titles) {
        tracks.push_back(getTrack(title));
    }
    return tracks;
}

TrackInfo InMemLibrary::getTrack(std::string title) { return titleToTrackMap[title]; }

std::vector<TrackInfo> InMemLibrary::getAllTracks() {
    std::vector<TrackInfo> tracks;
    for (auto i = titleToTrackMap.cbegin(); i != titleToTrackMap.cend(); ++i) {
        tracks.push_back(i->second);
    }
    return tracks;
}
