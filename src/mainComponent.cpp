#include "mainComponent.h"

MainComponent::MainComponent() {
    addAndMakeVisible(transport);
    addAndMakeVisible(trackAdder);
    addAndMakeVisible(library);
    configureElements();
    setSize(666, 666);
}

void MainComponent::configureTransport() {
    transport.setGain(0.25f);
    transport.addChangeListener(this);
}

void MainComponent::configureTrackAdder() {
    trackAdder.addChangeListener(this);
    trackAdder.setScannerWildcard(transport.getWildcardForAllFormats());
}

void MainComponent::configureLibrary() { library.addActionListener(this); }

void MainComponent::configureElements() {
    configureTransport();
    configureTrackAdder();
    configureLibrary();
}

void MainComponent::paint(juce::Graphics& g) {
    g.setColour(juce::Colours::black);
    g.fillAll();
}

void MainComponent::playTrack(TrackInfo track) {
    transport.loadTrack(track);
    transport.startPlayback();
}

void MainComponent::resizeTrackAdder() {
    trackAdder.setSize(getWidth() / 10, 20);
    trackAdder.setTopLeftPosition(5, 1);
}

void MainComponent::resizeLibrary() {
    library.setSize(getWidth(), getHeight() * 0.8);
    library.setTopLeftPosition(0, trackAdder.getBottom() + 2);
}

void MainComponent::resizeTransport() {
    transport.setSize(getWidth() - (2 * transportPadding),
                      (getHeight() * .25) - transportPadding);
    transport.setTopLeftPosition(transportPadding,
                                 getHeight() - transport.getHeight() - 5);
}

void MainComponent::resized() {
    resizeTrackAdder();
    resizeLibrary();
    resizeTransport();
}

void MainComponent::handleTracksAdded() {
    auto files = trackAdder.getResults();
    auto tracks = fileProcessor.processFiles(files);
    library.addTracks(tracks);
}

void MainComponent::handleTransportChange() {
    // TODO This will need to change when skips are added, probably to action callback
    if (!transport.hasActiveTrack()) {
        if (!playQueue.empty()) {
            playTrack(playQueue.getNextTrack());
        } else {
            playTrack(library.getNextTrack());
        }
    }
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &trackAdder) {
        handleTracksAdded();
    } else if (source == &transport) {
        handleTransportChange();
    }
}

void MainComponent::handlePlayMessage() { playTrack(library.getSelectedTrack()); }

void MainComponent::handleQueueMessage() {
    playQueue.addTrack(library.getSelectedTrack());
    // If nothing is playing, play track added to queue
    if (!transport.hasActiveTrack())
        playTrack(playQueue.getNextTrack());
}

void MainComponent::actionListenerCallback(const juce::String& message) {
    if (message == library.playMessage) {
        handlePlayMessage();
    } else if (message == library.queueMessage) {
        handleQueueMessage();
    }
}
