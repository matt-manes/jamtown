#pragma once

#include <deque>
#include <vector>
#include "trackInfo.h"

class PlayQueue {
public:
    ~PlayQueue() = default;

    void addTrack(TrackInfo track);

    void addTracks(std::vector<TrackInfo> tracks);

    TrackInfo getNextTrack();

    bool empty();

    void clear();

    std::vector<TrackInfo> getTrackList();

    void remove(std::vector<TrackInfo> tracks);

private:
    std::deque<TrackInfo> queue;
};
