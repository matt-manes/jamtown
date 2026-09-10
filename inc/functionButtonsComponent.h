#pragma once

#include "addTracksComponent.h"
#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

/**
 * @brief Container component for holding function controls in a row.
 *
 */
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
    float maxButtonWidth = 150;
    float minButtonWidth = 20;
    juce::FlexItem::Margin leftButtonMargin{0, 1, 0, 0};
    juce::FlexItem::Margin midButtonMargin{0, 1, 0, 1};
    juce::FlexItem::Margin rightButtonMargin{0, 0, 0, 1};
    juce::FlexBox layoutBox;

    void viewLibraryClicked();
    void viewPlayQueueClicked();

    /**
     * @brief Do all set up here and call from constructor.
     *
     */
    void initializeComponent();

    void configureTrackAdder();
    void configureViewLibrary();
    void configureViewPlayQueue();

    /**
     * @brief Creates a FlexItem for a function button with the given margin.
     *
     * @param component
     * @param margin
     * @return juce::FlexItem
     */
    juce::FlexItem componentToFlexItem(juce::Component& component,
                                       juce::FlexItem::Margin margin);

    /**
     * @brief Create a FlexItem for a function button using `midButtonMargin`.
     *
     * @param component
     * @return juce::FlexItem
     */
    juce::FlexItem componentToFlexItem(juce::Component& component);

    void configureLayout();
    void applyButtonStyle(juce::TextButton& button);
};
