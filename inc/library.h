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

    /**
     * @brief Add the given track to the library.
     *
     * @param track
     */
    virtual void addTrack(TrackInfo track) = 0;

    /**
     * @brief Add the given tracks to the library.
     *
     * @param tracks
     */
    virtual void addTracks(std::vector<TrackInfo> tracks);

    /**
     * @brief Get the tracklists for all albums by the given artist.
     *
     * @param artist
     * @return std::unordered_map<std::string, std::vector<TrackInfo>> A map of album titles to tracklist.
     */
    virtual std::unordered_map<std::string, std::vector<TrackInfo>> getAlbumsByArtist(
        std::string artist) = 0;

    /**
     * @brief Get all album titles.
     *
     * @return std::vector<std::string>
     */
    virtual std::vector<std::string> getAllAlbumTitles() = 0;

    /**
     * @brief Get all artist names.
     *
     * @return std::vector<std::string>
     */
    virtual std::vector<std::string> getAllArtistTitles() = 0;

    /**
     * @brief Get the tracklist for the given album and artist.
     *
     * @param album
     * @param artist
     * @return std::vector<TrackInfo>
     */
    virtual std::vector<TrackInfo> getAlbumTracks(std::string album,
                                                  std::string artist) = 0;

    /**
     * @brief Get all tracks by the given artist.
     *
     * @param artist
     * @return std::vector<TrackInfo>
     */
    virtual std::vector<TrackInfo> getArtistTracks(std::string artist);

    /**
     * @brief Get the track for the given title, album, and artist.
     *
     * @param title
     * @param album
     * @param artist
     * @return TrackInfo
     */
    virtual TrackInfo getTrack(std::string title,
                               std::string album,
                               std::string artist) = 0;

    /**
     * @brief Get all tracks in the library.
     *
     * @return std::vector<TrackInfo>
     */
    virtual std::vector<TrackInfo> getAllTracks() = 0;

    /**
     * @brief Remove the track for the given title, album, and artist from the library.
     *
     * @param title
     * @param album
     * @param artist
     */
    virtual void removeTrack(std::string title,
                             std::string album,
                             std::string artist) = 0;

    /**
     * @brief Remove the given track from the library.
     *
     * @param track
     */
    virtual void removeTrack(TrackInfo track);

    /**
     * @brief Remove the given tracks from the library.
     *
     * @param tracks
     */
    virtual void removeTracks(std::vector<TrackInfo> tracks);

    /**
     * @brief Get a random track from the library.
     *
     * @return TrackInfo
     */
    virtual TrackInfo getRandomTrack() = 0;

    /**
     * @brief Get the tracklist for a random album.
     *
     * @return std::vector<TrackInfo>
     */
    virtual std::vector<TrackInfo> getRandomAlbumTracks() = 0;
};
