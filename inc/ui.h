#pragma once

#include "transportComponent.h"
#include "addTracksComponent.h"
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>
#include <memory>
#include "fileProcessor.h"

class MainContentComponent : public juce::Component, public juce::ChangeListener {
 public:
    MainContentComponent() {
        addAndMakeVisible(transport);
        transport.setGain(0.25f);
        addAndMakeVisible(trackAdder);
        trackAdder.addChangeListener(this);
        trackAdder.setScannerWildcard(transport.getWildcardForAllFormats());
        setSize(666, 666);
    }

    void paint(juce::Graphics& g) override {
        g.setColour(juce::Colours::black);
        g.fillAll();
    }

    void resized() override {
        trackAdder.setSize(getWidth() / 5, 20);
        trackAdder.setTopLeftPosition(5, 0);
        transport.setSize(getWidth() - (2 * transportPadding),
                          (getHeight() - trackAdder.getHeight()) -
                              (2 * transportPadding));
        transport.setTopLeftPosition(transportPadding,
                                     trackAdder.getBottom() + transportPadding);
    }

    void changeListenerCallback(juce::ChangeBroadcaster* source) {
        if (source == &trackAdder) {
            auto files = trackAdder.getResults();
            auto tracks = fileProcessor.processFiles(files);
            for (auto track : tracks) {
                std::cout << track.toString() << std::endl;
            }
        }
    }

 private:
    TransportComponent transport;
    AddTracksComponent trackAdder;
    FileProcessor fileProcessor;
    int transportPadding = 10;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainContentComponent)
};
