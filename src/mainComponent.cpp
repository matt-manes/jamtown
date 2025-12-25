#include "mainComponent.h"

MainComponent::MainComponent() : transportComponent(&transport) {
    configureActionHandlers();
    configureElements();
    setSize(666, 666);
}

void MainComponent::configureTransport() {
    addAndMakeVisible(transportComponent);
    transport.addChangeListener(&transportComponent);
    transport.stop();
    transportComponent.addChangeListener(this);
    transportComponent.addActionListener(this);
}

void MainComponent::configureTopBar() {
    addAndMakeVisible(topBar);
    topBar.addActionListener(this);
    topBar.setTrackAdderWildcard(transport.getWildcardForAllFormats());
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
    transport.loadTrack(track);
    transport.start();
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
    if (!transport.hasActiveTrack()) {
        if (!playQueue.empty()) {
            playTrack(playQueue.getNextTrack());
            sendActionMessage(ActionMessages::playQueueUpdated);
        } else {
            playTrack(browser.getNextLibraryTrack(transport.getCurrentTrack()));
        }
    }
}

void MainComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &transportComponent) {
        handleTransportChange();
    }
}

void MainComponent::handleLoadSelectedMessage() {
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
    if (!transport.hasActiveTrack())
        playTrack(playQueue.getNextTrack());
    sendActionMessage(ActionMessages::playQueueUpdated);
}

void MainComponent::handlePauseMessage() { transport.pause(); }

void MainComponent::handlePlayMessage() { transport.start(); }

void MainComponent::handleStopMessage() { transport.stop(); }

void MainComponent::handleViewLibraryMessage() { browser.setView(View::LIBRARY); }

void MainComponent::handleViewPlayQueueMessage() { browser.setView(View::PLAYQUEUE); }

void MainComponent::configureActionHandlers() {
    actionHandlers.emplace(ActionMessages::loadSelectedTracks,
                           [this] { handleLoadSelectedMessage(); });
    actionHandlers.emplace(ActionMessages::pauseTrack, [this] { handlePauseMessage(); });
    actionHandlers.emplace(ActionMessages::playTrack, [this] { handlePlayMessage(); });
    actionHandlers.emplace(ActionMessages::stopTrack, [this] { handleStopMessage(); });
    actionHandlers.emplace(ActionMessages::queueTrack, [this] { handleQueueMessage(); });
    actionHandlers.emplace(ActionMessages::filesForLibrary,
                           [this] { handleTracksAdded(); });
    actionHandlers.emplace(ActionMessages::viewLibrary,
                           [this] { handleViewLibraryMessage(); });
    actionHandlers.emplace(ActionMessages::viewPlayQueue,
                           [this] { handleViewPlayQueueMessage(); });
}

void MainComponent::actionListenerCallback(const juce::String& message) {
    if (actionHandlers.contains(message))
        actionHandlers[message]();
}
