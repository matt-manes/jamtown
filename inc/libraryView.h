#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_events/juce_events.h>
#include <vector>
#include <string>
#include <memory>
#include "inMemLibrary.h"
#include "trackInfo.h"
#include "playQueue.h"
#include "browserView.h"

/**
 * @brief BrowserView implementation for track library.
 *
 */
class LibraryView : public BrowserView {
public:
    LibraryView() { configureHeaders(); }
    ~LibraryView() = default;

    void configureHeaders() override;

    void cellDoubleClicked(int rowNumber,
                           int columnId,
                           const juce::MouseEvent& mouseEvent) override;

    void cellClicked(int rowNumber,
                     int columnId,
                     const juce::MouseEvent& mouseEvent) override;

    TrackInfo getNextTrack(TrackInfo currentTrack);

    void sortOrderChanged(int newSortColumnId, bool isForwards) override;

    void setTracklist(std::vector<TrackInfo> tracks) override;
};
