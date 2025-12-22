#include "mainComponent.h"

MainComponent::MainComponent() {
    configureElements();
    setSize(666, 666);
}

void MainComponent::configureTransport() {
    addAndMakeVisible(transportComponent);
    transportComponent.setGain(0.25f);
    transportComponent.addChangeListener(this);
}

void MainComponent::configureTopBar() {
    addAndMakeVisible(topBar);
    topBar.addActionListener(this);
    topBar.setTrackAdderWildcard(transportComponent.getWildcardForAllFormats());
}

void MainComponent::configureBrowser() {
    addAndMakeVisible(browser);
    browser.addActionListener(this);
    browser.setLibrary(&library);
    browser.setPlayQueue(&playQueue);
    addActionListener(&browser);
}

void MainComponent::configureElements() {
    configureTransport();
    configureBrowser();
    configureTopBar();
}

void MainComponent::paint(juce::Graphics& g) {
    g.setColour(juce::Colours::black);
    g.fillAll();
}

void MainComponent::playTrack(TrackInfo track) {
    transportComponent.loadTrack(track);
    transportComponent.startPlayback();
}

void MainComponent::resizeTopBar() {
    topBar.setSize(getWidth() - 2, 20);
    topBar.setTopLeftPosition(1, 1);
}

void MainComponent::resizeBrowser() {
    browser.setSize(getWidth(), static_cast<int>(getHeight() * 0.8));
    browser.setTopLeftPosition(0, topBar.getBottom() + 2);
}

void MainComponent::resizeTransport() {
    transportComponent.setSize(getWidth() - (2 * transportPadding),
                               static_cast<int>(getHeight() * .25) - transportPadding);
    transportComponent.setTopLeftPosition(
        transportPadding, getHeight() - transportComponent.getHeight() - 5);
}

void MainComponent::resized() {
    resizeTopBar();
    resizeBrowser();
    resizeTransport();
}

void MainComponent::handleTracksAdded() {
    auto files = topBar.getTrackAdderFiles();
    auto tracks = fileProcessor.processFiles(files);
    library.addTracks(tracks);
    sendActionMessage(ActionMessages::libraryUpdated);
}

void MainComponent::handleTransportChange() {
    // TODO This will need to change when skips are added, probably to action callback
    if (!transportComponent.hasActiveTrack()) {
        if (!playQueue.empty()) {
            playTrack(playQueue.getNextTrack());
            sendActionMessage(ActionMessages::playQueueUpdated);
        } else {
            // playTrack(library.getNextTrack());
            //  TODO add browser next track method
            playTrack(browser.getNextLibraryTrack(transportComponent.getCurrentTrack()));
            // playTrack(browser.getSelectedTrack());
        }
    }
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &transportComponent) {
        handleTransportChange();
    }
}

void MainComponent::handlePlayMessage() {
    auto tracks = browser.getSelectedTracks();
    playTrack(tracks[0]);
    // If multiple tracks were selected, add the rest to the queue
    if (tracks.size() > 1) {
        for (auto i = (tracks.begin() + 1); i != tracks.end(); ++i) {
            playQueue.addTrack(*i);
        }
        sendActionMessage(ActionMessages::playQueueUpdated);
    }
}

void MainComponent::handleQueueMessage() {
    for (auto i : browser.getSelectedTracks()) {
        playQueue.addTrack(i);
    }
    // playQueue.addTrack(browser.getSelectedTrack());
    //  If nothing is playing, play track added to queue
    if (!transportComponent.hasActiveTrack())
        playTrack(playQueue.getNextTrack());
    sendActionMessage(ActionMessages::playQueueUpdated);
}

void MainComponent::actionListenerCallback(const juce::String& message) {
    // TODO replace with map
    if (message == ActionMessages::playTrack) {
        handlePlayMessage();
    } else if (message == ActionMessages::queueTrack) {
        handleQueueMessage();
    } else if (message == ActionMessages::filesForLibrary) {
        handleTracksAdded();
    } else if (message == ActionMessages::viewLibrary) {
        browser.setView(View::LIBRARY);
    } else if (message == ActionMessages::viewPlayQueue) {
        browser.setView(View::PLAYQUEUE);
    }
}
