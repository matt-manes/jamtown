#include "playQueueView.h"

#include "actionMessages.h"

// TODO abstract to a "trackListingView" base class inheriting from `BrowserView`
void PlayQueueView::configureHeaders() {
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

// TODO abstract to a "trackListingView" base class inheriting from `BrowserView`
void PlayQueueView::cellDoubleClicked(int rowNumber,
                                      int /*columnId*/,
                                      const juce::MouseEvent& mouseEvent) {
    if (mouseEvent.mods.isLeftButtonDown() && rowNumber < getNumRows()) {
        // selectedTrack = trackList[rowNumber];
        // selectedTrackIndex = rowNumber;
        sendActionMessage(ActionMessages::playTrack);
    }
}

void PlayQueueView::cellClicked(int rowNumber,
                                int /*columnId*/,
                                const juce::MouseEvent& mouseEvent) {
    if (mouseEvent.mods.isRightButtonDown() && rowNumber < getNumRows()) {
        juce::PopupMenu menu;
        menu.addItem(1, "Play");
        // menu.addItem(2, "Add to playlist", false);
        menu.showMenuAsync(juce::PopupMenu::Options(), [this, rowNumber](int result) {
            switch (result) {
            case 1:
                // selectedTrackIndex = rowNumber;
                sendActionMessage(ActionMessages::playTrack);
                break;
            // TODO implement other options
            default:
                break;
            }
        });
    }
}
