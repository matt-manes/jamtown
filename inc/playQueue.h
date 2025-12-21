#pragma once

#include <deque>
#include <vector>
#include "trackInfo.h"

class PlayQueue {
public:
    ~PlayQueue() = default;

    void addTrack(TrackInfo track);

    TrackInfo getNextTrack();

    bool empty() { return queue.empty(); }

    void clear() { queue.clear(); }

    std::vector<TrackInfo> getTrackList();

private:
    std::deque<TrackInfo> queue;
};
