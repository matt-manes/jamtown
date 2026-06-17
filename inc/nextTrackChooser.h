#pragma once

#include "trackInfo.h"

class NextTrackChooser {
public:
    virtual TrackInfo getNextTrack() = 0;
};

class NonEmptyPlayQueueTrackChooser : public NextTrackChooser {};
