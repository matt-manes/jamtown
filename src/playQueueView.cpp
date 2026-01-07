#include "playQueueView.h"
#include "actionMessages.h"

PlayQueueView::PlayQueueView() { configureHeaders(); }

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
        "Title", 1, 100, 10, -1, juce::TableHeaderComponent::notSortable);
    table.getHeader().addColumn(
        "Album", 2, 100, 10, -1, juce::TableHeaderComponent::notSortable);
    table.getHeader().addColumn(
        "Artist", 3, 100, 10, -1, juce::TableHeaderComponent::notSortable);
    table.getHeader().addColumn(
        "Length", 4, 100, 10, -1, juce::TableHeaderComponent::notSortable);
    table.setMultipleSelectionEnabled(true);
}

// TODO abstract to a "trackListingView" base class inheriting from `BrowserView`
void PlayQueueView::cellDoubleClicked(int rowNumber,
                                      int /*columnId*/,
                                      const juce::MouseEvent& mouseEvent) {
    if (mouseEvent.mods.isLeftButtonDown() && rowNumber < getNumRows()) {
        // TODO remove selected track from playqueue probably by moving it to the front
        sendActionMessage(ActionMessages::loadSelectedTracks);
    }
}

void PlayQueueView::showContextMenu(int /*rowNumber*/) {
    juce::PopupMenu menu;
    menu.addItem("Play", [this]() {
        // TODO adjust playqueue on selection
        sendActionMessage(ActionMessages::loadSelectedTracks);
    });
    menu.addItem("Remove from queue", [this]() {
        sendActionMessage(ActionMessages::removeSelectedFromPlayQueue);
    });
    menu.showMenuAsync(juce::PopupMenu::Options());
}

void PlayQueueView::cellClicked(int rowNumber,
                                int /*columnId*/,
                                const juce::MouseEvent& mouseEvent) {
    if (mouseEvent.mods.isRightButtonDown() && rowNumber < getNumRows()) {
        showContextMenu(rowNumber);
    }
}
