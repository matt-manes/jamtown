#include "playQueue.h"

void PlayQueue::addTrack(TrackInfo track) { queue.push_back(track); }

TrackInfo PlayQueue::getNextTrack() {
    TrackInfo track = queue.front();
    queue.pop_front();
    return track;
}

bool PlayQueue::empty() { return queue.empty(); }

void PlayQueue::clear() { queue.clear(); }

std::vector<TrackInfo> PlayQueue::getTrackList() {
    return std::vector<TrackInfo>{queue.cbegin(), queue.cend()};
}
