#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_events/juce_events.h>
#include "browserView.h"

/**
 * @brief BrowserView implementation for viewing the next up tracks.
 *
 */
class PlayQueueView : public BrowserView {
public:
    PlayQueueView() { configureHeaders(); }
    ~PlayQueueView() = default;

    void configureHeaders() override;
    void cellDoubleClicked(int rowNumber,
                           int columnId,
                           const juce::MouseEvent& mouseEvent) override;

    void cellClicked(int rowNumber,
                     int columnId,
                     const juce::MouseEvent& mouseEvent) override;
};
