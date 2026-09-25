#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

/**
 * @brief Label component for displaying info about tracks currently visible in the browser.
 *
 */
class VisibleTracksDataComponent : public juce::Label {
public:
    VisibleTracksDataComponent() {}

    void update(int numTracks, double totalTime);
};
