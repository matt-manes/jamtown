#include "searchService.h"
#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include <mutex>
#include <vector>
#include <string>

std::int64_t SearchService::getID(TrackInfo track) {
    auto path = track.getStringPath();
    if (filePathsToIdMap.contains(path))
        // If track has already been added, just return its id
        return filePathsToIdMap[path];
    auto id = nextID;
    idMap[id] = track;
    filePathsToIdMap[path] = id;
    ++nextID;
    return id;
}

TrackInfo SearchService::getTrackFromID(std::int64_t id) { return idMap[id]; }

void SearchService::indexString(std::string searchable, std::int64_t trackID) {
    for (auto c : searchable) {
        index[c].insert(trackID);
    }
}

void SearchService::addTrack(TrackInfo track) {
    auto id = getID(track);
    if (id == -1)
        return;
    indexString(track.getNormalizedArtist(), id);
    indexString(track.getNormalizedAlbum(), id);
    indexString(track.getNormalizedTitle(), id);
}

void SearchService::addTracks(std::vector<TrackInfo> tracks) {
    for (auto track : tracks)
        addTrack(track);
}

void SearchService::clearSearch() {
    results.clear();
    query = "";
}

bool SearchService::searchInProgress() { return !query.empty(); }

void SearchService::initializeResults() {
    for (int i = 0; i < nextID; ++i)
        results.insert(i);
}

void SearchService::search(std::string substring) {
    if (!searchInProgress()) {
        initializeResults();
    }
    query += substring;
    for (auto c : substring) {
        if (!index.contains(c)) {
            // There are no results that match the character,
            // i.e. no track has this character and the result set will be empty no matter what.
            results.clear();
            return;
        }
        updateResults(index[c]);
    }
}

void search(std::string sub) {
    query += sub;
    for (auto c : sub) {
        if (!index.contains(c))
            continue;
        std::unordered_set<std::int64_t> tmp;
        for (auto id : results)
            if (ids.contains(id))
                tmp.insert(id);
        for (auto id : ids)
            if (results.contains(id))
                tmp.insert(id);
        results = tmp;
    }
}

void SearchService::updateResults(std::unordered_set<std::int64_t> ids) {
    // Do an intersection
    std::unordered_set<std::int64_t> tmp;
    for (auto id : results)
        if (ids.contains(id))
            tmp.insert(id);
    for (auto id : ids)
        if (results.contains(id))
            tmp.insert(id);
    results = tmp;
}

bool SearchService::textInTrackInfo(std::string text, TrackInfo track) {
    return track.getNormalizedArtist().contains(text) ||
           track.getNormalizedTitle().contains(text) ||
           track.getNormalizedAlbum().contains(text);
}

std::vector<TrackInfo> SearchService::getResults() {
    std::unordered_set<std::int64_t> result_ids;
    std::string searchQuery;
    result_ids = results;
    searchQuery = query;
    std::vector<TrackInfo> tracks;
    // don't bother checking for query in track
    // if query is empty or 1 character
    if (searchQuery.size() < 2) {
        for (auto id : result_ids)
            tracks.push_back(getTrackFromID(id));
        return tracks;
    }
    for (auto id : result_ids) {
        TrackInfo track = getTrackFromID(id);
        if (textInTrackInfo(searchQuery, track))
            tracks.push_back(track);
    }
    return tracks;
}

void SearchService::removeTrack(TrackInfo track) {
    auto id = getID(track);
    for (auto pair : index) {
        index[pair.first].erase(id);
    }
    results.erase(id);
}

void SearchService::removeTracks(std::vector<TrackInfo> tracks) {
    for (auto track : tracks)
        removeTrack(track);
}

void SearchService::recomputeSearch() {
    auto previousQuery = query;
    clearSearch();
    search(previousQuery);
}
