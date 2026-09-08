#include "mainComponent.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <algorithm>
#include <vector>
#include <string>
#include <memory>
#include "actionMessages.h"

MainComponent::MainComponent()
    : transportComponent(&transportController), topBar(&searchService) {
    configureActionHandlers();
    configureElements();
    libraryPersistanceService = std::make_unique<TxtLibraryPersistanceService>();
    loadLibrary();
    addActionListener(this);
}

void MainComponent::loadLibrary() {
    // Using `loadingLib` and `libLoaded`
    // to prevent leaks if user exits
    // while library is being loaded
    loadingLib = true;
    juce::Thread::launch([this]() {
        auto tracks = libraryPersistanceService->load();
        library.addTracks(tracks);
        searchService.addTracks(tracks);
        libLoaded = true;
        sendActionMessage(ActionMessages::libraryUpdated);
    });
}

void MainComponent::configureTransport() {
    addAndMakeVisible(transportComponent);
    transportController.addChangeListener(&transportComponent);
    transportController.stop();
    transportComponent.addChangeListener(this);
    transportComponent.addActionListener(this);
}

void MainComponent::configureTopBar() {
    addAndMakeVisible(topBar);
    topBar.addFunctionButtonsActionListener(this);
    topBar.setTrackAdderWildcard(transportController.getWildcardForAllFormats());
    topBar.addSearchBoxActionListener(this);
}

void MainComponent::configureBrowser() {
    addAndMakeVisible(browser);
    browser.addActionListener(this);
    browser.setLibrary(&library);
    browser.setPlayQueue(&playQueue);
    addActionListener(&browser);
}

void MainComponent::configureLayout() {
    layoutBox.flexDirection = juce::FlexBox::Direction::column;
    layoutBox.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    layoutBox.items.add(
        juce::FlexItem(topBar).withFlex(0, 1, topBarHeight).withMargin(columnItemMargin));
    layoutBox.items.add(juce::FlexItem(browser).withFlex(1).withMargin(columnItemMargin));
    layoutBox.items.add(juce::FlexItem(transportComponent)
                            .withFlex(0, 1, transportComponentHeight)
                            .withMargin(juce::FlexItem::Margin{0, 0, 10, 0}));
}

void MainComponent::configureElements() {
    configureTransport();
    configureBrowser();
    configureTopBar();
    configureLayout();
}

void MainComponent::paint(juce::Graphics& g) {
    g.setColour(juce::Colours::black);
    g.fillAll();
}

void MainComponent::playTrack(TrackInfo track) {
    transportController.loadTrack(track);
    transportController.start();
    browser.setCurrentlyPlayingTrack(track);
}

void MainComponent::resized() { layoutBox.performLayout(getLocalBounds()); }

void MainComponent::handleTracksAdded() {
    auto files = topBar.getTrackAdderFiles();
    auto tracks = fileProcessor.processFiles(files);
    library.addTracks(tracks);
    libraryPersistanceService->save(library.getAllTracks());
    searchService.addTracks(tracks);
    sendActionMessage(ActionMessages::libraryUpdated);
}

TrackInfo MainComponent::getRandomTrackToPlay() {
    if (library.getTrackCount() <= 1)
        return transportController.getCurrentTrack();
    auto track = library.getRandomTrack();
    while (track == transportController.getCurrentTrack())
        track = library.getRandomTrack();
    return track;
}

std::vector<TrackInfo> MainComponent::getRandomAlbumToPlay() {
    auto tracks = library.getRandomAlbumTracks();
    if (library.getAllAlbumTitles().size() <= 1)
        return tracks;
    while (tracks[0].getAlbum() == transportController.getCurrentTrack().getAlbum() &&
           tracks[0].getArtist() == transportController.getCurrentTrack().getArtist())
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
            playTrack(
                browser.getNextLibraryViewTrack(transportController.getCurrentTrack()));
        else if (shuffleMode == ShuffleMode::TRACK)
            playTrack(getRandomTrackToPlay());
        else if (shuffleMode == ShuffleMode::ALBUM)
            overwritePlayQueue(getRandomAlbumToPlay(), "Title");
    }
}

void MainComponent::handleTransportChange() {
    // TODO This will need to change when skips are added, probably to action callback
    if (!transportController.hasActiveTrack()) {
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
    if (!transportController.hasActiveTrack())
        transportController.loadTrack(playQueue.getNextTrack());
    sendActionMessage(ActionMessages::playQueueUpdated);
}

void MainComponent::handlePauseMessage() { transportController.pause(); }

void MainComponent::handlePlayMessage() { transportController.start(); }

void MainComponent::handleStopMessage() { transportController.stop(); }

void MainComponent::handleViewLibraryMessage() { browser.setView(View::LIBRARY); }

void MainComponent::handleViewPlayQueueMessage() { browser.setView(View::PLAYQUEUE); }

void MainComponent::handleNextTrackMessage() { playNextTrack(); }

void MainComponent::handleRestartTrackMessage() { transportController.setPosition(0.0); }

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
    libraryPersistanceService->save(library.getAllTracks());
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
    if (!transportController.hasActiveTrack() && playQueue.empty()) {
        if (transportComponent.getCurrentShuffleMode() == ShuffleMode::TRACK) {
            transportController.loadTrack(getRandomTrackToPlay());
        } else if (transportComponent.getCurrentShuffleMode() == ShuffleMode::ALBUM) {
            overwritePlayQueue(getRandomAlbumToPlay(), "Title");
            // overwrite playqueue starts playing when called
            transportController.stop();
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
    if (searchService.getQuery().empty()) {
        browser.updateLibraryViewTrackList(library.getAllTracks());
    } else {
        auto tracks = searchService.getResults();
        browser.updateLibraryViewTrackList(tracks);
    }
}

void MainComponent::handleLibraryUpdatedMessage() {
    // Update search results when library updated if the search box isn't empty
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
