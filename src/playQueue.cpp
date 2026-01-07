#include "playQueue.h"
#include "trackInfo.h"
#include <vector>
#include <unordered_set>
#include <string>
#include <algorithm>

void PlayQueue::addTrack(TrackInfo track) { queue.push_back(track); }

void PlayQueue::addTracks(std::vector<TrackInfo> tracks) {
    for (auto track : tracks) {
        addTrack(track);
    }
}

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

void PlayQueue::remove(std::vector<TrackInfo> tracks) {
    std::unordered_set<std::string> trackPaths;
    for (auto track : tracks)
        trackPaths.insert(track.getPath().getFullPathName().toStdString());
    queue.erase(std::remove_if(queue.begin(),
                               queue.end(),
                               [trackPaths](TrackInfo t) {
                                   return trackPaths.contains(
                                       t.getPath().getFullPathName().toStdString());
                               }),
                queue.end());
}
