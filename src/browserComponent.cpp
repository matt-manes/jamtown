#include "browserComponent.h"

BrowserComponent::BrowserComponent() {
    libraryView = std::make_unique<LibraryView>();
    playQueueView = std::make_unique<PlayQueueView>();
    addChildComponent(libraryView.get());
    addChildComponent(playQueueView.get());
    libraryView->addActionListener(this);
    playQueueView->addActionListener(this);
    setView(LIBRARY);
}

void BrowserComponent::setView(View view) {
    if (currentView != nullptr)
        currentView->setVisible(false);
    switch (view) {
    case LIBRARY:
        currentView = libraryView.get();
        break;
    case PLAYQUEUE:
        currentView = playQueueView.get();
        break;
    default:
        break;
    }
    currentView->setVisible(true);
}

TrackInfo BrowserComponent::getNextLibraryTrack(TrackInfo currentTrack) {
    return libraryView->getNextTrack(currentTrack);
}

void BrowserComponent::resized() {
    libraryView->setBounds(0, 0, getWidth(), getHeight());
    playQueueView->setBounds(0, 0, getWidth(), getHeight());
}

std::vector<TrackInfo> BrowserComponent::getSelectedTracks() {
    return currentView->getSelectedTracks();
}

void BrowserComponent::actionListenerCallback(const juce::String& message) {
    if (message == ActionMessages::libraryUpdated) {
        libraryView->setTracklist(library->getAllTracks());
    } else if (message == ActionMessages::playQueueUpdated) {
        playQueueView->setTracklist(playQueue->getTrackList());
    } else  // propogate action messages sent from view objects
        sendActionMessage(message);
}

void BrowserComponent::setLibrary(Library* newLibrary) {
    library = newLibrary;
    libraryView->setTracklist(library->getAllTracks());
}

void BrowserComponent::setPlayQueue(PlayQueue* newQueue) {
    playQueue = newQueue;
    playQueueView->setTracklist(playQueue->getTrackList());
}

void BrowserComponent::setCurrentlyPlayingTrack(TrackInfo track) {
    libraryView->setCurrentlyPlayingTrack(track);
}

std::string BrowserComponent::getAlbumToPlay() { return libraryView->getAlbumToPlay(); }
std::string BrowserComponent::getArtistToPlay() { return libraryView->getArtistToPlay(); }
