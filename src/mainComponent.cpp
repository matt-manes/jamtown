#include "mainComponent.h"
#include "actionMessages.h"
#include <algorithm>
#include <vector>
#include <string>

MainComponent::MainComponent() : transportComponent(&transport) {
    configureActionHandlers();
    configureElements();
    loadLibrary();
    // Careful with this
    // don't wanna cause recursion oblivion
    // messaging set up should be refactored to avoid this
    addActionListener(this);
}

void MainComponent::loadLibrary() {
    // Using `loadingLib` and `libLoaded`
    // to prevent leaks if user exits
    // while library is being loaded
    loadingLib = true;
    juce::Thread::launch([this]() {
        auto tracks = LibReader::read();
        library.addTracks(tracks);
        searchService.addTracks(tracks);
        libLoaded = true;
        sendActionMessage(ActionMessages::libraryUpdated);
    });
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

void MainComponent::configureSearchBox() {
    addAndMakeVisible(searchBox);
    searchBox.searchService = &searchService;
    searchBox.addActionListener(this);
}

void MainComponent::configureElements() {
    configureTransport();
    configureBrowser();
    configureTopBar();
    configureSearchBox();
}

void MainComponent::paint(juce::Graphics& g) {
    g.setColour(juce::Colours::black);
    g.fillAll();
}

void MainComponent::playTrack(TrackInfo track) {
    transport.loadTrack(track);
    transport.start();
    browser.setCurrentlyPlayingTrack(track);
}

void MainComponent::resizeTopBar() {
    topBar.setSize(static_cast<int>(getWidth() * 0.75), 20);
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

void MainComponent::resizeSearchBox() {
    searchBox.setSize(static_cast<int>(getWidth() * 0.25), 20);
    searchBox.setTopRightPosition(getWidth() - 1, 1);
}

void MainComponent::resized() {
    resizeTopBar();
    resizeBrowser();
    resizeTransport();
    resizeSearchBox();
}

void MainComponent::handleTracksAdded() {
    auto files = topBar.getTrackAdderFiles();
    auto tracks = fileProcessor.processFiles(files);
    library.addTracks(tracks);
    LibWriter::write(library.getAllTracks());
    searchService.addTracks(tracks);
    sendActionMessage(ActionMessages::libraryUpdated);
}

TrackInfo MainComponent::getRandomTrackToPlay() {
    if (library.getTrackCount() <= 1)
        return transport.getCurrentTrack();
    auto track = library.getRandomTrack();
    while (track == transport.getCurrentTrack())
        track = library.getRandomTrack();
    return track;
}

std::vector<TrackInfo> MainComponent::getRandomAlbumToPlay() {
    auto tracks = library.getRandomAlbumTracks();
    if (library.getAllAlbumTitles().size() <= 1)
        return tracks;
    while (tracks[0].getAlbum() == transport.getCurrentTrack().getAlbum() &&
           tracks[0].getArtist() == transport.getCurrentTrack().getArtist())
        tracks = library.getRandomAlbumTracks();
    return tracks;
}

void MainComponent::playNextTrack() {
    if (!playQueue.empty()) {
        playTrack(playQueue.getNextTrack());
        sendActionMessage(ActionMessages::playQueueUpdated);
    } else {
        auto shuffleMode = transportComponent.getCurrentShuffleMode();
        if (shuffleMode == ShuffleMode::OFF)
            playTrack(browser.getNextLibraryViewTrack(transport.getCurrentTrack()));
        else if (shuffleMode == ShuffleMode::TRACK)
            playTrack(getRandomTrackToPlay());
        else if (shuffleMode == ShuffleMode::ALBUM)
            overwritePlayQueue(getRandomAlbumToPlay(), "Title");
    }
}

void MainComponent::handleTransportChange() {
    // TODO This will need to change when skips are added, probably to action callback
    if (!transport.hasActiveTrack()) {
        playNextTrack();
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
    playQueue.addTracks(browser.getSelectedTracks());
    //  If nothing is playing, load track added to queue
    if (!transport.hasActiveTrack())
        transport.loadTrack(playQueue.getNextTrack());
    sendActionMessage(ActionMessages::playQueueUpdated);
}

void MainComponent::handlePauseMessage() { transport.pause(); }

void MainComponent::handlePlayMessage() { transport.start(); }

void MainComponent::handleStopMessage() { transport.stop(); }

void MainComponent::handleViewLibraryMessage() { browser.setView(View::LIBRARY); }

void MainComponent::handleViewPlayQueueMessage() { browser.setView(View::PLAYQUEUE); }

void MainComponent::handleNextTrackMessage() { playNextTrack(); }

void MainComponent::handleRestartTrackMessage() { transport.setPosition(0.0); }

void MainComponent::handlePlayAlbumMessage() {
    auto selection = browser.getAlbumToPlay();
    auto tracks = library.getAlbumTracks(selection.first, selection.second);
    overwritePlayQueue(tracks, "Title");
}

void MainComponent::handlePlayArtistMessage() {
    auto tracks = library.getArtistTracks(browser.getArtistToPlay());
    overwritePlayQueue(tracks, "Album");
}

void MainComponent::handleRemoveFromLibraryMessage() {
    auto tracks = browser.getSelectedTracks();
    library.removeTracks(tracks);
    LibWriter::write(library.getAllTracks());
    searchService.removeTracks(tracks);
    sendActionMessage(ActionMessages::libraryUpdated);
    // TODO update playqueue if it contains removed tracks
}

void MainComponent::handleDeleteFromHarddriveMessage() {
    auto tracks = browser.getSelectedTracks();
    for (auto track : tracks) {
        track.getPath().deleteFile();
    }
    // TODO refactor this, shouldn't be calling handle message
    // TODO functions not in response to a message
    handleRemoveFromLibraryMessage();
}

void MainComponent::handleShuffleModeChangedMessage() {
    if (!transport.hasActiveTrack() && playQueue.empty()) {
        if (transportComponent.getCurrentShuffleMode() == ShuffleMode::TRACK) {
            transport.loadTrack(getRandomTrackToPlay());
        } else if (transportComponent.getCurrentShuffleMode() == ShuffleMode::ALBUM) {
            overwritePlayQueue(getRandomAlbumToPlay(), "Title");
            // overwrite playqueue starts playing when called
            transport.stop();
        }
    }
}

void MainComponent::handlePlayRandomAlbumMessage() {
    overwritePlayQueue(getRandomAlbumToPlay(), "Title");
}

void MainComponent::handleRemoveSelectedFromPlayQueueMessage() {
    auto selectedTracks = browser.getSelectedTracks();
    playQueue.remove(selectedTracks);
    sendActionMessage(ActionMessages::playQueueUpdated);
}

void MainComponent::handleSearchUpdatedMessage() {
    std::cout << searchService.getQuery() << std::endl;
    if (searchService.getQuery().empty())
        browser.updateLibraryViewTrackList(library.getAllTracks());
    else {
        auto tracks = searchService.getResults();
        for (auto track : tracks) {
            std::cout << track.toString() << std::endl;
        }
        browser.updateLibraryViewTrackList(tracks);
    }
}

void MainComponent::handleLibraryUpdatedMessage() {
    // TODO refactor this, this is whack
    if (!searchService.getQuery().empty()) {
        searchService.recomputeSearch();
        handleSearchUpdatedMessage();
    }
}

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
    actionHandlers.emplace(ActionMessages::nextTrack,
                           [this] { handleNextTrackMessage(); });
    actionHandlers.emplace(ActionMessages::restartTrack,
                           [this] { handleRestartTrackMessage(); });
    actionHandlers.emplace(ActionMessages::playAlbum,
                           [this] { handlePlayAlbumMessage(); });
    actionHandlers.emplace(ActionMessages::playArtist,
                           [this] { handlePlayArtistMessage(); });
    actionHandlers.emplace(ActionMessages::removeTracksFromLibrary,
                           [this] { handleRemoveFromLibraryMessage(); });
    actionHandlers.emplace(ActionMessages::deleteTracksFromHarddrive,
                           [this] { handleDeleteFromHarddriveMessage(); });
    actionHandlers.emplace(ActionMessages::shuffleModeChanged,
                           [this] { handleShuffleModeChangedMessage(); });
    actionHandlers.emplace(ActionMessages::playRandomAlbum,
                           [this] { handlePlayRandomAlbumMessage(); });
    actionHandlers.emplace(ActionMessages::removeSelectedFromPlayQueue,
                           [this] { handleRemoveSelectedFromPlayQueueMessage(); });
    actionHandlers.emplace(ActionMessages::searchUpdated,
                           [this] { handleSearchUpdatedMessage(); });
    actionHandlers.emplace(ActionMessages::libraryUpdated,
                           [this] { handleLibraryUpdatedMessage(); });
}

void MainComponent::actionListenerCallback(const juce::String& message) {
    if (actionHandlers.contains(message))
        actionHandlers[message]();
}

void MainComponent::overwritePlayQueue(std::vector<TrackInfo> tracks,
                                       std::string sortAttribute) {
    playQueue.clear();
    if (!sortAttribute.empty()) {
        TrackSorter::sort(tracks, sortAttribute, true);
    }
    playQueue.addTracks(tracks);
    playTrack(playQueue.getNextTrack());
    sendActionMessage(ActionMessages::playQueueUpdated);
}
