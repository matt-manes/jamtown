#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "browserView.h"

/**
 * @brief BrowserView implementation for viewing the next up tracks.
 *
 */
class PlayQueueView : public BrowserView {
public:
    PlayQueueView();
    ~PlayQueueView() = default;

    void configureHeaders() override;

    void cellDoubleClicked(int rowNumber,
                           int columnId,
                           const juce::MouseEvent& mouseEvent) override;

    void cellClicked(int rowNumber,
                     int columnId,
                     const juce::MouseEvent& mouseEvent) override;

private:
    /**
     * @brief Show the context menu to the user.
     *
     * @param rowNumber
     */
    void showContextMenu(int rowNumber);
};
