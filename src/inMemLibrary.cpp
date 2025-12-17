#include "inMemLibrary.h"

void InMemLibrary::addTrack(TrackInfo track) {
    titleToTrackMap[track.getArtist()] = track;
    albumToTrackTitleMap[track.getAlbum()].push_back(track.getTitle());
    artistToAlbumMap[track.getArtist()].push_back(track.getAlbum());
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
