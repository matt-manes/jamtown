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
    static void write(std::vector<TrackInfo> tracks);
    static std::string prepareTrackInfo(TrackInfo track);
};

class LibReader {
public:
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

    size_t getTrackCount() { return tracks.size(); }

private:
    std::unordered_set<std::string> filepaths;
    std::vector<TrackInfo> tracks;
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::vector<TrackInfo>>>
        db;
    juce::Random random;
};
