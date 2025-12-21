#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>
#include "fileScanner.h"

/**
 * @brief Button for selecting folder to add tracks from.
 *
 */
class AddTracksComponent : public juce::Component, public juce::ChangeBroadcaster {
public:
    AddTracksComponent();

    ~AddTracksComponent() override = default;

    void paint(juce::Graphics& g) override;

    void resized() override;

    /**
     * @brief Get the files from folder scanning.
     *
     * @return juce::Array<juce::File>
     */
    juce::Array<juce::File> getResults();

    /**
     * @brief Set the wildcard pattern for playable files.
     *
     * @param wildcard
     */
    void setScannerWildcard(juce::String wildcard);

private:
    juce::TextButton addTracksButton;
    juce::Array<juce::File> gatheredFiles;
    std::unique_ptr<juce::FileChooser> chooser;
    juce::File lastOpened;
    FileScanner fileScanner;
    juce::String supportedFormatsWildcard;

    void onClick();
};
