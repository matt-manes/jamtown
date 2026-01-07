#include "browserView.h"
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_events/juce_events.h>
#include <string>
#include <vector>

int BrowserView::getNumRows() { return static_cast<int>(tracklist.size()); }

void BrowserView::paintRowBackground(juce::Graphics& g,
                                     int /*rowNumber*/,
                                     int /*width*/,
                                     int /*height*/,
                                     bool rowIsSelected) {
    if (rowIsSelected)
        g.fillAll(juce::Colours::hotpink);
    else
        g.fillAll(juce::Colours::cornflowerblue);
}

void BrowserView::paintCell(juce::Graphics& g,
                            int rowNumber,
                            int columnId,
                            int width,
                            int height,
                            bool /*rowIsSelected*/) {
    g.setColour(juce::Colours::black);
    g.fillRect(width - 1, 0, 1, height);
    g.fillRect(0, height - 1, width, 1);
    if (rowNumber < getNumRows()) {
        TrackInfo track = getTrack(rowNumber);
        auto text = getColumnText(rowNumber, columnId);
        g.setColour(juce::Colours::black);
        g.setFont(juce::Font(14.5, 0));
        g.drawText(text, 1, 0, width - 1, height, juce::Justification::centredLeft);
    }
}

std::string BrowserView::getColumnText(int rowNumber, int columnId) {
    TrackInfo track = getTrack(rowNumber);
    switch (columnId) {
    case 1:
        return track.getTitle();
    case 2:
        return track.getAlbum();
    case 3:
        return track.getArtist();
    case 4:
        return track.getLengthString();
    default:
        return "";
    }
}

void BrowserView::resized() {
    table.setBounds(0, 0, getWidth(), getHeight());
    table.getHeader().resizeAllColumnsToFit(getWidth());
}

void BrowserView::setTracklist(std::vector<TrackInfo> tracks) {
    tracklist = tracks;
    table.deselectAllRows();
    table.updateContent();
}

void BrowserView::cellDoubleClicked(int /*rowNumber*/,
                                    int /*columnId*/,
                                    const juce::MouseEvent& /*mouseEvent*/) {
    return;
}

void BrowserView::cellClicked(int /*rowNumber*/,
                              int /*columnId*/,
                              const juce::MouseEvent& /*mouseEvent*/) {
    return;
}

std::vector<TrackInfo> BrowserView::getSelectedTracks() {
    std::vector<TrackInfo> tracks;
    for (int i = 0; i < table.getNumSelectedRows(); ++i) {
        tracks.push_back(tracklist[table.getSelectedRow(i)]);
    }
    return tracks;
}

TrackInfo BrowserView::getTrack(int index) { return tracklist[index]; }
