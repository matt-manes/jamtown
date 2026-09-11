#pragma once

#include <vector>
#include <string>
#include "playQueue.h"
#include "transportComponent.h"
#include "browserComponent.h"
#include "transportController.h"
#include "library.h"
#include "juce_events/juce_events.h"

class TrackController : public juce::ChangeBroadcaster {
public:
    TrackController(Library& library,
                    PlayQueue& playQueue,
                    BrowserComponent& browser,
                    TransportController& transportController,
                    TransportComponent& transportComponent)
        : library(library), playQueue(playQueue), browser(browser),
          transportController(transportController),
          transportComponent(transportComponent) {}

    void playNextTrack();

    void overwritePlayQueue(std::vector<TrackInfo> tracks, std::string sortAttribute);

    void playRandomAlbum();

private:
    Library& library;
    PlayQueue& playQueue;
    BrowserComponent& browser;
    TransportController& transportController;
    TransportComponent& transportComponent;

    TrackInfo getRandomTrackToPlay();

    std::vector<TrackInfo> getRandomAlbumToPlay();
};
