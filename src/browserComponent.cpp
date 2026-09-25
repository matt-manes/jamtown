#include "browserComponent.h"
#include "actionMessages.h"
#include <memory>
#include <vector>
#include <utility>
#include <string>

BrowserComponent::BrowserComponent()
    : libraryView(std::make_unique<LibraryView>()),
      playQueueView(std::make_unique<PlayQueueView>()) {
    initializeComponent();
}

void BrowserComponent::setView(View view) {
    if (currentView != nullptr)
        // hide whatever the current view is before switching to new one
        currentView->setVisible(false);
    // TODO replace with a map of enums to views
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
    sendChangeMessage();
}

TrackInfo BrowserComponent::getNextLibraryViewTrack(TrackInfo currentTrack) {
    return libraryView->getNextTrack(currentTrack);
}

void BrowserComponent::resized() {
    auto bounds = getLocalBounds();
    auto bottom = bounds.removeFromBottom(20);
    visibleTrackDataComponent.setBounds(bottom);
    libraryView->setBounds(bounds);
    playQueueView->setBounds(bounds);
}

std::vector<TrackInfo> BrowserComponent::getSelectedTracks() {
    return currentView->getSelectedTracks();
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
    libraryView->scrollTrackIntoView(track);
}

std::pair<std::string, std::string> BrowserComponent::getAlbumToPlay() {
    return libraryView->getAlbumToPlay();
}
std::string BrowserComponent::getArtistToPlay() { return libraryView->getArtistToPlay(); }

void BrowserComponent::updateLibraryViewTrackList(std::vector<TrackInfo> tracks) {
    libraryView->setTracklist(tracks);
}

void BrowserComponent::updatePlayQueueViewTrackList(std::vector<TrackInfo> tracks) {
    playQueueView->setTracklist(tracks);
}

int BrowserComponent::getNumVisibleTracks() { return currentView->getNumRows(); }

double BrowserComponent::getTotalTimeOfVisibleTracks() {
    return currentView->getTotalTrackListTime();
}

void BrowserComponent::updateVisibleTrackDataComponent() {
    visibleTrackDataComponent.update(getNumVisibleTracks(),
                                     getTotalTimeOfVisibleTracks());
}

void BrowserComponent::initializeComponent() {
    // Don't make them visible
    addChildComponent(libraryView.get());
    addChildComponent(playQueueView.get());

    configureVisibleTrackDataComponent();
    addAndMakeVisible(visibleTrackDataComponent);
    setView(LIBRARY);
}

void BrowserComponent::configureVisibleTrackDataComponent() {
    visibleTrackDataComponent.setColour(juce::Label::textColourId,
                                        juce::Colours::hotpink);
}
