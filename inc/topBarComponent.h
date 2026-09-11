#pragma once

#include "functionButtonsComponent.h"
#include "searchService.h"
#include "searchBoxComponent.h"
#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

/**
 * @brief Component containing the function buttons component and the search bar component.
 *
 */
class TopBarComponent : public juce::Component {
public:
    TopBarComponent(SearchService* searchService) : searchBox(searchService) {
        initializeComponent();
    };
    ~TopBarComponent() = default;

    void resized() override;

    void configureLayout();

    /**
     * @brief Set up should be done here and called from constructor.
     *
     */
    void initializeComponent();

    /**
     * @brief Regsiter the given listener with the function buttons component.
     *
     * @param listener
     */
    void addFunctionButtonsActionListener(juce::ActionListener* listener);

    /**
     * @brief Set the wildcard pattern to use when adding files to the library.
     *
     * @param wildcard A ';' seperated list of file extension wildcards.
     */
    void setTrackAdderWildcard(juce::String wildcard);

    /**
     * @brief Register the given listener with the search box component.
     *
     * @param listener
     */
    void addSearchBoxActionListener(juce::ActionListener* listener);

    void addSearchBoxChangeListener(juce::ChangeListener* listener);

    /**
     * @brief Get the files found during the last scan.
     *
     * @return juce::Array<juce::File>
     */
    juce::Array<juce::File> getTrackAdderFiles();

    void addTrackAdderListener(juce::ChangeListener* listener);

private:
    FunctionButtonsComponent functionButtons;
    SearchBoxComponent searchBox;
    juce::FlexBox layoutBox;
};
