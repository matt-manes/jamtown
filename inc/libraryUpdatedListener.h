#pragma once

#include <juce_events/juce_events.h>
#include "libraryPersistanceService.h"
#include "searchService.h"
#include "browserComponent.h"

/**
 * @brief Persists and refreshes the library after it changes.
 *
 * Saves the current track list to storage and updates the visible library or search
 * results whenever a library update notification is broadcast.
 */
class LibraryUpdatedListener : public juce::ChangeListener {
public:
    LibraryUpdatedListener(Library& library,
                           LibraryPersistanceService& libraryPersistanceService,
                           BrowserComponent& browserComponent,
                           SearchService& searchService)
        : library(library), libraryPersistanceService(libraryPersistanceService),
          browserComponent(browserComponent), searchService(searchService) {}

    void changeListenerCallback(juce::ChangeBroadcaster* /*source*/) override {
        libraryPersistanceService.save(library.getAllTracks());
        if (!searchService.getQuery().empty()) {
            searchService.recomputeSearch();
            browserComponent.updateLibraryViewTrackList(searchService.getResults());
        } else {
            browserComponent.updateLibraryViewTrackList(library.getAllTracks());
        }
    }

private:
    Library& library;
    LibraryPersistanceService& libraryPersistanceService;
    BrowserComponent& browserComponent;
    SearchService& searchService;
};
