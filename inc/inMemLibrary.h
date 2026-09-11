#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "trackInfo.h"
#include "library.h"
#include <juce_core/juce_core.h>

/**
 * @brief In memory implementation of `Library` interface.
 *
 */
class InMemLibrary : public Library {
public:
    void addTrack(TrackInfo track) override;

    void addTracks(std::vector<TrackInfo> tracks) override;

    std::unordered_map<std::string, std::vector<TrackInfo>> getAlbumsByArtist(
        std::string artist) override;

    std::vector<std::string> getAllAlbumTitles() override;

    std::vector<std::string> getAllArtistTitles() override;

    std::vector<TrackInfo> getAlbumTracks(std::string album, std::string artist) override;

    TrackInfo getTrack(std::string title, std::string album, std::string artist) override;

    std::vector<TrackInfo> getAllTracks() override;

    void removeTrack(std::string title, std::string album, std::string artist) override;

    void removeTracks(std::vector<TrackInfo> tracks) override;

    TrackInfo getRandomTrack() override;

    std::vector<TrackInfo> getRandomAlbumTracks() override;

    size_t getTrackCount() override { return trackList.size(); }

private:
    // Used to determine if a track is already in the library
    std::unordered_set<std::string> filepaths;
    // Maintain all tracks in a vector so it doesn't have to be gotten
    // through iterating db map whenever it's needed
    std::vector<TrackInfo> trackList;
    // First key is artist name, the second key is the album title
    // Innermost value is the tracklist of the album
    // i.e. `std::vector<TrackInfo> tracks = db[artist][album];`
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::vector<TrackInfo>>>
        db;
    juce::Random random;

    void addTrackWithNoBroadcast(TrackInfo track);

    void removeTrackWithNoBroadcast(std::string title,
                                    std::string album,
                                    std::string artist);
};
