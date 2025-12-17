#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "trackInfo.h"
#include "library.h"

/**
 * @brief In memory implementation of `Library` interface.
 *
 */
class InMemLibrary : public Library {
 public:
    void addTrack(TrackInfo track) override;

    std::unordered_map<std::string, std::vector<TrackInfo>> getAlbums(
        std::string artist) override;

    std::vector<TrackInfo> getAlbum(std::string album) override;

    TrackInfo getTrack(std::string title) override;

    std::vector<TrackInfo> getAllTracks() override;

 private:
    std::unordered_map<std::string, TrackInfo> titleToTrackMap;
    std::unordered_map<std::string, std::unordered_set<std::string>> albumToTrackTitleMap;
    std::unordered_map<std::string, std::unordered_set<std::string>> artistToAlbumMap;
};
