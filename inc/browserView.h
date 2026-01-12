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

    int getNumRows() override;

    /**
     * @brief Configure the headers to be used in this view.
     *
     */
    virtual void configureHeaders() = 0;

    void paintRowBackground(juce::Graphics& g,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected) override;

    void paintCell(juce::Graphics& g,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;

    /**
     * @brief Set the tracklist to display.
     *
     * @param tracks
     */
    virtual void setTracklist(std::vector<TrackInfo> tracks);

    /**
     * @brief Get the track at the given index.
     *
     * @param index
     * @return TrackInfo
     */
    virtual TrackInfo getTrack(int index);

    /**
     * @brief Get the currently selected tracks.
     *
     * @return std::vector<TrackInfo>
     */
    virtual std::vector<TrackInfo> getSelectedTracks();

    /**
     * @brief Get the text displayed in the given row of the given column.
     *
     * @param rowNumber
     * @param columnId
     * @return std::string
     */
    virtual std::string getColumnText(int rowNumber, int columnId);

    virtual void resized() override;

    virtual void cellDoubleClicked(int rowNumber,
                                   int columnId,
                                   const juce::MouseEvent& mouseEvent) override;

    virtual void cellClicked(int rowNumber,
                             int columnId,
                             const juce::MouseEvent& mouseEvent) override;

protected:
    juce::TableListBox table{{}, this};
    std::vector<TrackInfo> tracklist;
};
