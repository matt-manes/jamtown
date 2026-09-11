#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>
#include <juce_graphics/juce_graphics.h>
#include "searchService.h"
#include "actionMessages.h"
#include <string>

/**
 * @brief Component allowing a user to search their library.
 *
 */
class SearchBoxComponent : public juce::TextEditor,
                           public juce::ActionBroadcaster,
                           public juce::ChangeBroadcaster {
public:
    SearchBoxComponent(SearchService* searchService);
    ~SearchBoxComponent() = default;

    /**
     * @brief Callback for `onTextChange` member.
     * Passes the current text to the search service and sends a 'searchUpdated' action message.
     *
     */
    void onTextChangeCallback();

    /**
     * @brief Set up should be done here and called from the constructor.
     *
     */
    void initializeComponent();

private:
    SearchService* searchService;
    std::string lastEntry;
};
