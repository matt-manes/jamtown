#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "trackInfo.h"
#include "library.h"
#include <juce_core/juce_core.h>

/* Temp quick and dirty library persistence.*/
class LibWriter {
public:
    /**
     * @brief Save the given tracklist to disk.
     *
     * @param tracks
     */
    static void write(std::vector<TrackInfo> tracks);

    /**
     * @brief Format the given track for writing.
     *
     * @param track
     * @return std::string
     */
    static std::string prepareTrackInfo(TrackInfo track);
};

class LibReader {
public:
    /**
     * @brief Load track info from disk.
     *
     * @return std::vector<TrackInfo>
     */
    static std::vector<TrackInfo> read();
};

/**
 * @brief In memory implementation of `Library` interface.
 *
 */
class InMemLibrary : public Library {
public:
    void addTrack(TrackInfo track) override;

    std::unordered_map<std::string, std::vector<TrackInfo>> getAlbumsByArtist(
        std::string artist) override;

    std::vector<std::string> getAllAlbumTitles() override;

    std::vector<std::string> getAllArtistTitles() override;

    std::vector<TrackInfo> getAlbumTracks(std::string album, std::string artist) override;

    TrackInfo getTrack(std::string title, std::string album, std::string artist) override;

    std::vector<TrackInfo> getAllTracks() override;

    void removeTrack(std::string title, std::string album, std::string artist) override;

    TrackInfo getRandomTrack() override;

    std::vector<TrackInfo> getRandomAlbumTracks() override;

    /**
     * @brief Get the number of tracks in the library.
     *
     * @return size_t
     */
    size_t getTrackCount() { return tracks.size(); }

private:
    // Used to determine if a track is already in the library
    std::unordered_set<std::string> filepaths;
    // Maintain all tracks in a vector so it doesn't have to be gotten
    // through iterating db map whenever it's needed
    std::vector<TrackInfo> tracks;
    // First key is artist name, the second key is the album title
    // Innermost value is the tracklist of the album
    // i.e. `std::vector<TrackInfo> tracks = db[artist][album];`
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::vector<TrackInfo>>>
        db;
    juce::Random random;
};
