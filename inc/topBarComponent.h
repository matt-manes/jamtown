#pragma once

#include "functionButtonsComponent.h"
#include "searchService.h"
#include "searchBoxComponent.h"
#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

class TopBarComponent : public juce::Component {
public:
    TopBarComponent(SearchService* searchService) : searchBox(searchService) {
        initializeComponent();
    };
    ~TopBarComponent() = default;

    void resized() override;

    void configureLayout();

    void initializeComponent();

    void addFunctionButtonsActionListener(juce::ActionListener* listener);

    void setTrackAdderWildcard(juce::String wildcard);

    void addSearchBoxActionListener(juce::ActionListener* listener);

    /**
     * @brief Get the files found during the last scan.
     *
     * @return juce::Array<juce::File>
     */
    juce::Array<juce::File> getTrackAdderFiles();

private:
    FunctionButtonsComponent functionButtons;
    SearchBoxComponent searchBox;
    juce::FlexBox layoutBox;
};
