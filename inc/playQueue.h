#pragma once

#include <deque>
#include <vector>
#include "trackInfo.h"

/**
 * @brief Provides queueing for tracks.
 *
 */
class PlayQueue {
public:
    ~PlayQueue() = default;

    /**
     * @brief Add the given track to the queue.
     *
     * @param track
     */
    void addTrack(TrackInfo track);

    /**
     * @brief Add the given tracks to the queue.
     *
     * @param tracks
     */
    void addTracks(std::vector<TrackInfo> tracks);

    /**
     * @brief Removes and returns the next track in the queue.
     *
     * @return TrackInfo
     */
    TrackInfo getNextTrack();

    /**
     * @brief Whether the queue is emtpy.
     *
     * @return true
     * @return false
     */
    bool empty();

    /**
     * @brief Clear the queue contents.
     *
     */
    void clear();

    /**
     * @brief Get the tracks in this queue without affecting the queue.
     *
     * @return std::vector<TrackInfo>
     */
    std::vector<TrackInfo> getTrackList();

    /**
     * @brief Remove the given tracks from the queue.
     *
     * @param tracks
     */
    void remove(std::vector<TrackInfo> tracks);

private:
    std::deque<TrackInfo> queue;
};
