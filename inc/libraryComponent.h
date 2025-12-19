#pragma once

#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_events/juce_events.h>
#include <vector>
#include <string>
#include "inMemLibrary.h"
#include "trackInfo.h"

class LibraryComponent : public juce::Component,
                         public juce::TableListBoxModel,
                         public juce::ActionBroadcaster {
public:
    LibraryComponent();

    ~LibraryComponent() = default;

    int getNumRows() override { return trackList.size(); }

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

    void addTracks(std::vector<TrackInfo> tracks) {
        library.addTracks(tracks);
        trackList = library.getAllTracks();
        table.updateContent();
    }

    TrackInfo getSelectedTrack() { return selectedTrack; }

    TrackInfo getNextTrack();

    void resized() override;

    void cellDoubleClicked(int rowNumber,
                           int columnId,
                           const juce::MouseEvent& me) override;

    void cellClicked(int rowNumber, int columnId, const juce::MouseEvent& me) override;

    const juce::String playMessage = "PLAY TRACK";
    const juce::String queueMessage = "QUEUE TRACK";

private:
    juce::TableListBox table{{}, this};
    InMemLibrary library;
    std::vector<TrackInfo> trackList;
    TrackInfo selectedTrack;
    int selectedTrackIndex = -1;

    std::string getColumnText(int columnId, TrackInfo track);
};
