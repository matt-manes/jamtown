#pragma once

#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include <mutex>
#include <vector>
#include <string>
#include "trackInfo.h"
#include <juce_core/juce_core.h>

class SearchService {
public:
    SearchService() {}

    void addTrack(TrackInfo track);

    void addTracks(std::vector<TrackInfo> tracks);

    void clearSearch();

    void search(std::string substring);

    std::vector<TrackInfo> getResults();

    std::string getQuery() { return query; }

    void removeTrack(TrackInfo track);

    void removeTracks(std::vector<TrackInfo> tracks);

    void recomputeSearch();

private:
    // maps characters to the set of track info ids that contain that character
    std::unordered_map<char, std::unordered_set<std::int64_t>> index;
    // hold matching track info ids for the current search
    std::unordered_set<std::int64_t> results;
    // current search query
    std::string query = "";
    // maps ids to track info instances
    std::unordered_map<std::int64_t, TrackInfo> idMap;
    // for retrieving ids from tracks
    std::unordered_map<std::string, std::int64_t> filePathsToIdMap;
    std::int64_t nextID = 0;

    /*
     * currently, the index building is single threaded
     * so we only need mutexes for searching related structures,
     * could change in the future
     ? assuming juce text box callback is multithreaded
     ? if not we can get rid of mutexes
     ? kinda lookin like this is the case
     ? unless I explicity launch a thread
     ? instead of handling callback on messaging thread
     ? depends on search performance
     */

    std::mutex searchMutex;

    /**
     * @brief Get a new id number for the given track.
     * If the track already exists, -1 will be returned.
     *
     * @param track
     * @return std::int64_t
     */
    std::int64_t getID(TrackInfo track);

    TrackInfo getTrackFromID(std::int64_t id);

    void indexString(std::string searchable, std::int64_t trackID);

    bool searchInProgress();

    void initializeResults();

    void updateResults(std::unordered_set<std::int64_t> ids);

    bool textInTrackInfo(std::string text, TrackInfo track);
};
