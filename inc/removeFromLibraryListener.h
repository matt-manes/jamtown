#pragma once

#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>
#include "searchService.h"
#include "browserComponent.h"
#include "actionMessages.h"

class RemoveFromLibraryListener : public juce::ActionListener {
public:
    RemoveFromLibraryListener(Library& library,
                              BrowserComponent& browserComponent,
                              SearchService& searchService)
        : library(library), browserComponent(browserComponent),
          searchService(searchService) {}

    void actionListenerCallback(const juce::String& message) override {
        if (message == ActionMessages::removeTracksFromLibrary) {
            auto tracks = browserComponent.getSelectedTracks();
            searchService.removeTracks(tracks);
            library.removeTracks(tracks);
        }
    }

private:
    Library& library;
    BrowserComponent& browserComponent;
    SearchService& searchService;
};
