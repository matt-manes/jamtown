#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_events/juce_events.h>
#include <vector>
#include <string>
#include "trackInfo.h"

/**
 * @brief Base class for a view that can be displayed by the browser component.
 *
 */
class BrowserView : public juce::Component,
                    public juce::TableListBoxModel,
                    public juce::ActionBroadcaster {
public:
    ~BrowserView() = default;

    virtual int getNumRows() override;

    virtual void configureHeaders() = 0;

    virtual void paintRowBackground(juce::Graphics& g,
                                    int rowNumber,
                                    int width,
                                    int height,
                                    bool rowIsSelected) override;

    virtual void paintCell(juce::Graphics& g,
                           int rowNumber,
                           int columnId,
                           int width,
                           int height,
                           bool rowIsSelected) override;

    virtual void setTracklist(std::vector<TrackInfo> tracks);

    virtual TrackInfo getTrack(int index);

    virtual std::vector<TrackInfo> getSelectedTracks();

    virtual std::string getColumnText(int rowNumber, int columnId);

    virtual void resized() override;

    virtual void cellDoubleClicked(int rowNumber,
                                   int columnId,
                                   const juce::MouseEvent& mouseEvent) override;

    virtual void cellClicked(int rowNumber,
                             int columnId,
                             const juce::MouseEvent& mouseEvent) override;

    void updateTable();

protected:
    juce::TableListBox table{{}, this};
    std::vector<TrackInfo> tracklist;
};
