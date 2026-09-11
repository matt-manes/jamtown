#pragma once

#include <juce_events/juce_events.h>
#include "library.h"
#include "libraryPersistanceService.h"
#include "searchService.h"
#include "topBarComponent.h"
#include "browserComponent.h"
#include "fileProcessor.h"
#include "actionMessages.h"

class TracksAddedListener : public juce::ChangeListener, public juce::ActionBroadcaster {
public:
    TracksAddedListener(TopBarComponent& topBarComponent,
                        Library& library,
                        LibraryPersistanceService& libraryPersistanceService,
                        SearchService& searchService,
                        BrowserComponent& browserComponent)
        : topBarComponent(topBarComponent), library(library),
          libraryPersistanceService(libraryPersistanceService),
          searchService(searchService), browserComponent(browserComponent) {}

    void changeListenerCallback(juce::ChangeBroadcaster* /*source*/) override {
        auto files = topBarComponent.getTrackAdderFiles();
        auto tracks = fileProcessor.processFiles(files);
        library.addTracks(tracks);
        searchService.addTracks(tracks);
    }

private:
    TopBarComponent& topBarComponent;
    FileProcessor fileProcessor;
    Library& library;
    LibraryPersistanceService& libraryPersistanceService;
    SearchService& searchService;
    BrowserComponent& browserComponent;
};
