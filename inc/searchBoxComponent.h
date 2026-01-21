#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include "searchService.h"
#include "actionMessages.h"
#include <string>

class SearchBoxComponent : public juce::TextEditor, public juce::ActionBroadcaster {
public:
    SearchBoxComponent();
    ~SearchBoxComponent() = default;

    SearchService* searchService;

    void searchUpdated();

private:
    std::string lastEntry;
};
