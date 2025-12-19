#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "trackInfo.h"

/**
 * @brief Library service interface.
 *
 */
class Library {
public:
    virtual ~Library() = default;

    virtual void addTrack(TrackInfo track) = 0;

    virtual void addTracks(std::vector<TrackInfo> tracks);

    virtual std::unordered_map<std::string, std::vector<TrackInfo>> getAlbums(
        std::string artist) = 0;

    virtual std::vector<TrackInfo> getAlbum(std::string album) = 0;

    virtual std::vector<TrackInfo> getTracks(std::string artist);

    virtual TrackInfo getTrack(std::string title) = 0;

    virtual std::vector<TrackInfo> getAllTracks() = 0;
};
