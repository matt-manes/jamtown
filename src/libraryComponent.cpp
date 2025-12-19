#include "libraryComponent.h"

LibraryComponent::LibraryComponent() {
    addAndMakeVisible(table);
    table.getHeader().setStretchToFitActive(true);
    table.setColour(juce::ListBox::backgroundColourId, juce::Colours::black);
    table.setColour(juce::ListBox::textColourId, juce::Colours::black);
    table.getHeader().setColour(juce::TableHeaderComponent::backgroundColourId,
                                juce::Colours::aquamarine);
    table.setHeaderHeight(24);
    table.getHeader().addColumn(
        "Title", 1, 100, 10, -1, juce::TableHeaderComponent::defaultFlags);
    table.getHeader().addColumn(
        "Album", 2, 100, 10, -1, juce::TableHeaderComponent::defaultFlags);
    table.getHeader().addColumn(
        "Artist", 3, 100, 10, -1, juce::TableHeaderComponent::defaultFlags);
    table.getHeader().addColumn(
        "Length", 4, 100, 10, -1, juce::TableHeaderComponent::defaultFlags);
}

void LibraryComponent::paintRowBackground(juce::Graphics& g,
                                          int /*rowNumber*/,
                                          int /*width*/,
                                          int /*height*/,
                                          bool rowIsSelected) {
    if (rowIsSelected)
        g.fillAll(juce::Colours::hotpink);
    else
        g.fillAll(juce::Colours::cornflowerblue);
}

void LibraryComponent::paintCell(juce::Graphics& g,
                                 int rowNumber,
                                 int columnId,
                                 int width,
                                 int height,
                                 bool /*rowIsSelected*/) {
    g.setColour(juce::Colours::black);
    g.fillRect(width - 1, 0, 1, height);
    g.fillRect(0, height - 1, width, 1);
    if (rowNumber < getNumRows()) {
        TrackInfo track = trackList[rowNumber];
        auto text = getColumnText(columnId, track);
        g.setColour(juce::Colours::black);
        g.setFont(juce::Font(14.5, 0));
        g.drawText(text, 1, 0, width - 1, height, juce::Justification::centredLeft);
    }
}

std::string LibraryComponent::getColumnText(int columnId, TrackInfo track) {
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

void LibraryComponent::resized() {
    table.setBounds(0, 0, getWidth(), getHeight());
    table.getHeader().resizeAllColumnsToFit(getWidth());
}

void LibraryComponent::cellDoubleClicked(int rowNumber,
                                         int /*columnId*/,
                                         const juce::MouseEvent& me) {
    if (me.mods.isLeftButtonDown() && rowNumber < getNumRows()) {
        selectedTrack = trackList[rowNumber];
        selectedTrackIndex = rowNumber;
        sendActionMessage(playMessage);
    }
}

void LibraryComponent::cellClicked(int rowNumber,
                                   int /*columnId*/,
                                   const juce::MouseEvent& me) {
    if (me.mods.isRightButtonDown() && rowNumber < getNumRows()) {
        juce::PopupMenu menu;
        menu.addItem(1, "Play");
        menu.addItem(2, "Add to queue");
        menu.addItem(3, "Add to playlist", false);
        menu.showMenuAsync(juce::PopupMenu::Options(), [this, rowNumber](int result) {
            switch (result) {
            case 1:
                selectedTrack = trackList[rowNumber];
                selectedTrackIndex = rowNumber;
                sendActionMessage(playMessage);
                break;
            case 2:
                selectedTrack = trackList[rowNumber];
                selectedTrackIndex = rowNumber;
                sendActionMessage(queueMessage);
            // TODO implement other options
            default:
                break;
            }
        });
    }
}

TrackInfo LibraryComponent::getNextTrack() {
    selectedTrackIndex = (selectedTrackIndex + 1) % getNumRows();
    selectedTrack = trackList[selectedTrackIndex];
    return selectedTrack;
}
