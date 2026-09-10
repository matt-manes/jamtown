#pragma once

#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include <mutex>
#include <vector>
#include <string>
#include "trackInfo.h"
#include <juce_core/juce_core.h>

/**
 * @brief Provides case-insensitive partial matching search of track library.
 *
 * The search works by building an index that maps characters to ids of tracks that have that character
 * in either their artist name, album name, or title.
 * When a new character is added to the search query, the set of track ids that contain that character
 * is intersected with the previous result set, narrowing the results to ones that contain the full search query without
 * recomputing against the entire query.
 */
class SearchService {
public:
    SearchService() {}

    /**
     * @brief Add the given track to the search.
     *
     * @param track
     */
    void addTrack(TrackInfo track);

    /**
     * @brief Add the given tracks to the search.
     *
     * @param tracks
     */
    void addTracks(std::vector<TrackInfo> tracks);

    /**
     * @brief Clear the current search results.
     *
     */
    void clearSearch();

    /**
     * @brief Append the given substring to the current search query and update the results set.
     *
     * @param substring
     */
    void search(std::string substring);

    /**
     * @brief Get the result set for the current query.
     *
     * @return std::vector<TrackInfo>
     */
    std::vector<TrackInfo> getResults();

    /**
     * @brief Get the current query string.
     *
     * @return std::string
     */
    std::string getQuery() { return query; }

    /**
     * @brief Remove the given track from the searchable set.
     *
     * @param track
     */
    void removeTrack(TrackInfo track);

    /**
     * @brief Remove the given tracks from the searchabel set.
     *
     * @param tracks
     */
    void removeTracks(std::vector<TrackInfo> tracks);

    /**
     * @brief Recompute the result set for the current query.
     * Use when the set of searchable tracks has been changed during a search.
     *
     */
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

    /**
     * @brief Get a new id number for the given track.
     * If the track already exists, its id will be returned.
     *
     * @param track
     * @return std::int64_t
     */
    std::int64_t getID(TrackInfo track);

    /**
     * @brief Get the track that corresponds to the give id number.
     *
     * @param id
     * @return TrackInfo
     */
    TrackInfo getTrackFromID(std::int64_t id);

    /**
     * @brief Add `searchable` to the search index associated with `trackID`.
     *
     * @param searchable
     * @param trackID
     */
    void indexString(std::string searchable, std::int64_t trackID);

    /**
     * @brief Whether the current search query is empty or not.
     *
     * @return true
     * @return false
     */
    bool searchInProgress();

    /**
     * @brief Initializes the result set to hold all track ids.
     *
     */
    void initializeResults();

    /**
     * @brief Performs an intersection of `results` and the given set of ids, overwriting `results`.
     *
     * @param ids
     */
    void updateResults(std::unordered_set<std::int64_t> ids);

    /**
     * @brief Checks if the given string is a substring of either the artist, album, or title of the given track (case-insensitive).
     *
     * @param text
     * @param track
     * @return true
     * @return false
     */
    bool textInTrackInfo(std::string text, TrackInfo track);
};
