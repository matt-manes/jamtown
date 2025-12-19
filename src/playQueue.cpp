#include "playQueue.h"

void PlayQueue::addTrack(TrackInfo track) {
    queue.push(track);
    sendChangeMessage();
}

TrackInfo PlayQueue::getNextTrack() {
    TrackInfo track = queue.front();
    queue.pop();
    return track;
}
