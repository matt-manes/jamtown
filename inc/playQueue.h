#pragma once

#include <juce_events/juce_events.h>
#include <queue>
#include "trackInfo.h"

class PlayQueue : public juce::ChangeBroadcaster {
public:
    ~PlayQueue() = default;

    void addTrack(TrackInfo track);

    TrackInfo getNextTrack();

    bool empty() { return queue.empty(); }

    void clear() { queue = std::queue<TrackInfo>{}; }

private:
    std::queue<TrackInfo> queue;
};
