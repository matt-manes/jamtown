#pragma once

#include "addTracksComponent.h"
#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

class FunctionButtonsComponent : public juce::Component,
                                 public juce::ChangeListener,
                                 public juce::ActionBroadcaster {
public:
    FunctionButtonsComponent();
    ~FunctionButtonsComponent() = default;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void resized() override;

    /**
     * @brief Get the files found during the last scan.
     *
     * @return juce::Array<juce::File>
     */
    juce::Array<juce::File> getTrackAdderFiles();

    /**
     * @brief Set the wildcard to use when searching for files.
     *
     * @param wildcard A ';' seperated list of file extension wildcards.
     */
    void setTrackAdderWildcard(juce::String wildcard);

private:
    AddTracksComponent trackAdder;
    juce::TextButton viewLibrary;
    juce::TextButton viewPlayQueue;
    float buttonSpacing = 1;
    float maxButtonWidth = 150;
    float minButtonWidth = 20;
    juce::FlexItem::Margin leftButtonMargin{0, buttonSpacing, 0, 0};
    juce::FlexItem::Margin midButtonMargin{0, buttonSpacing, 0, buttonSpacing};
    juce::FlexItem::Margin rightButtonMargin{0, 0, 0, buttonSpacing};
    juce::FlexBox layoutBox;

    void viewLibraryClicked();
    void viewPlayQueueClicked();

    void initializeComponent();
    void configureTrackAdder();
    void configureViewLibrary();
    void configureViewPlayQueue();
    void configureLayout();
    void applyButtonStyle(juce::TextButton& button);
};
