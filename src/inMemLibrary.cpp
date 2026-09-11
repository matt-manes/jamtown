#include "inMemLibrary.h"
#include <unordered_map>
#include <string>
#include <vector>

void InMemLibrary::addTrackWithNoBroadcast(TrackInfo track) {
    std::string path = track.getStringPath();
    if (filepaths.contains(path))
        return;
    filepaths.insert(path);
    trackList.push_back(track);
    db[track.getArtist()][track.getAlbum()].push_back(track);
}

void InMemLibrary::addTrack(TrackInfo track) {
    addTrackWithNoBroadcast(track);
    sendChangeMessage();
}

void InMemLibrary::addTracks(std::vector<TrackInfo> tracks) {
    for (auto track : tracks)
        addTrackWithNoBroadcast(track);
    sendChangeMessage();
}

std::unordered_map<std::string, std::vector<TrackInfo>> InMemLibrary::getAlbumsByArtist(
    std::string artist) {
    if (db.contains(artist))
        return db[artist];
    return std::unordered_map<std::string, std::vector<TrackInfo>>{};
}

std::vector<std::string> InMemLibrary::getAllAlbumTitles() {
    std::vector<std::string> titles;
    // Not using getAllArtistTitles() b/c this will be faster than
    // copying them all to a vector first.
    for (auto artist : db) {
        for (auto album : artist.second) {
            titles.push_back(album.first);
        }
    }
    return titles;
}

std::vector<std::string> InMemLibrary::getAllArtistTitles() {
    std::vector<std::string> artists;
    for (auto artist : db) {
        artists.push_back(artist.first);
    }
    return artists;
}

std::vector<TrackInfo> InMemLibrary::getAlbumTracks(std::string album,
                                                    std::string artist) {
    if (db.contains(artist) && db[artist].contains(album))
        return db[artist][album];
    return std::vector<TrackInfo>{};
}

TrackInfo InMemLibrary::getTrack(std::string title,
                                 std::string album,
                                 std::string artist) {
    // Not ideal way to do look up time-complexity wise
    // but the number of tracks in an album is generally small
    auto albumTracks = getAlbumTracks(album, artist);
    for (auto track : albumTracks) {
        if (track.getTitle() == title) {
            return track;
        }
    }
    return TrackInfo{};
}

std::vector<TrackInfo> InMemLibrary::getAllTracks() { return trackList; }

void InMemLibrary::removeTrackWithNoBroadcast(std::string title,
                                              std::string album,
                                              std::string artist) {
    TrackInfo track = getTrack(title, album, artist);
    if (track.getTitle() == title) {
        filepaths.erase(track.getStringPath());
        std::erase_if(db[artist][album], [track](TrackInfo t) { return t == track; });
        std::erase_if(trackList, [track](TrackInfo t) { return t == track; });
        if (db[artist][album].empty())
            db[artist].erase(album);
        if (db[artist].empty())
            db.erase(artist);
    }
}

void InMemLibrary::removeTrack(std::string title, std::string album, std::string artist) {
    removeTrackWithNoBroadcast(title, album, artist);
    sendChangeMessage();
}

void InMemLibrary::removeTracks(std::vector<TrackInfo> tracks) {
    for (auto track : tracks)
        removeTrackWithNoBroadcast(track.getTitle(), track.getAlbum(), track.getArtist());
    sendChangeMessage();
}

TrackInfo InMemLibrary::getRandomTrack() {
    return trackList[(random.nextInt64() % trackList.size())];
}

std::vector<TrackInfo> InMemLibrary::getRandomAlbumTracks() {
    std::vector<std::vector<TrackInfo>> options;
    // Collect vectors of album tracks
    for (auto artist : db) {
        for (auto album : artist.second)
            options.push_back(album.second);
    }
    return options[(random.nextInt64() % options.size())];
}
