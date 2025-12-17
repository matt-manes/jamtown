#include "mainComponent.h"

MainComponent::MainComponent() {
    addAndMakeVisible(transport);
    transport.setGain(0.25f);
    addAndMakeVisible(trackAdder);
    trackAdder.addChangeListener(this);
    trackAdder.setScannerWildcard(transport.getWildcardForAllFormats());
    setSize(666, 666);
}

void MainComponent::paint(juce::Graphics& g) {
    g.setColour(juce::Colours::black);
    g.fillAll();
}

void MainComponent::resized() {
    trackAdder.setSize(getWidth() / 5, 20);
    trackAdder.setTopLeftPosition(5, 0);
    transport.setSize(getWidth() - (2 * transportPadding),
                      getHeight() - trackAdder.getHeight() - transportPadding);
    transport.setTopLeftPosition(transportPadding, trackAdder.getBottom());
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &trackAdder) {
        auto files = trackAdder.getResults();
        auto tracks = fileProcessor.processFiles(files);
        library.addTracks(tracks);
        auto track = library.getAllTracks()[0];
        transport.loadTrack(track);
    }
}
