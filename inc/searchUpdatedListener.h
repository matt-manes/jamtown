#pragma once

#include <juce_events/juce_events.h>
#include "searchService.h"
#include "browserComponent.h"

class SearchUpdatedListener : public juce::ChangeListener {
public:
    SearchUpdatedListener(Library& library,
                          BrowserComponent& browserComponent,
                          SearchService& searchService)
        : library(library), browserComponent(browserComponent),
          searchService(searchService) {}

    void changeListenerCallback(juce::ChangeBroadcaster* /*source*/) override {
        if (searchService.getQuery().empty()) {
            browserComponent.updateLibraryViewTrackList(library.getAllTracks());
        } else {
            auto tracks = searchService.getResults();
            browserComponent.updateLibraryViewTrackList(tracks);
        }
    }

private:
    Library& library;
    BrowserComponent& browserComponent;
    SearchService& searchService;
};
