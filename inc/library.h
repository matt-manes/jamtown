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

    virtual std::vector<TrackInfo> getAlbumTracks(std::string album,
                                                  std::string artist) = 0;

    virtual std::vector<TrackInfo> getArtistTracks(std::string artist);

    virtual TrackInfo getTrack(std::string title,
                               std::string album,
                               std::string artist) = 0;

    virtual std::vector<TrackInfo> getAllTracks() = 0;

    virtual void removeTrack(std::string title,
                             std::string album,
                             std::string artist) = 0;

    virtual void removeTrack(TrackInfo track);

    virtual void removeTracks(std::vector<TrackInfo> tracks);
};
