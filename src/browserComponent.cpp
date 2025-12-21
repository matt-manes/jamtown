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
