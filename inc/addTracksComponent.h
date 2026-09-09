#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>
#include "fileScanner.h"

/**
 * @brief Button for launching a folder selection dialog to add tracks to the library.
 *
 */
class AddTracksComponent : public juce::Component, public juce::ChangeBroadcaster {
public:
    AddTracksComponent();

    ~AddTracksComponent() override = default;

    void paint(juce::Graphics& g) override;

    void resized() override;

    /**
     * @brief Configure the `addTracksButton`.
     *
     */
    void configureAddTracksButton();

    /**
     * @brief Initialize the component. Should be called from constructor.
     *
     */
    void initializeComponent();

    /**
     * @brief Get the files found during the previous scan.
     * This function clears the results, i.e. subsequent calls will return an empty array unless another scan is performed.
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
    // The directory the last scan was executed from.
    juce::File lastOpened;
    FileScanner fileScanner;
    juce::String supportedFormatsWildcard;

    /**
     * @brief Launches the file chooser. Should be used as the callback for `addTracksButton.onClick`.
     *
     */
    void onClick();
};
