#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>
#include "fileScanner.h"

class AddTracksComponent : public juce::Component, public juce::ChangeBroadcaster {
 public:
    AddTracksComponent();

    ~AddTracksComponent() override = default;

    void paint(juce::Graphics& g) override;

    void resized() override;

    juce::Array<juce::File> getResults();

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
