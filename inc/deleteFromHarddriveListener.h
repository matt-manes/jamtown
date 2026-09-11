#pragma once

#include "juce_events/juce_events.h"
#include "juce_core/juce_core.h"
#include "actionMessages.h"
#include "browserComponent.h"
#include "library.h"
#include "searchService.h"

class DeleteFromHarddriveListener : juce::ActionListener {
public:
    DeleteFromHarddriveListener(BrowserComponent& browserComponent,
                                Library& library,
                                SearchService& searchService)
        : browserComponent(browserComponent), library(library),
          searchService(searchService) {}

    void actionListenerCallback(const juce::String& message) {
        if (message == ActionMessages::deleteTracksFromHarddrive) {
            auto tracks = browserComponent.getSelectedTracks();
            for (auto track : tracks) {
                track.getPath().deleteFile();
            }
            searchService.removeTracks(tracks);
            library.removeTracks(tracks);
        }
    }

private:
    BrowserComponent& browserComponent;
    Library& library;
    SearchService& searchService;
};
