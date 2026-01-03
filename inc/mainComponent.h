#pragma once

#include "transportComponent.h"
#include "addTracksComponent.h"
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>
#include <memory>
#include <unordered_map>
#include "playQueue.h"
#include "fileProcessor.h"
#include "inMemLibrary.h"
#include "browserComponent.h"
#include "transport.h"
#include "topBarComponent.h"

class MainComponent : public juce::Component,
                      public juce::ChangeListener,
                      public juce::ActionListener,
                      public juce::ActionBroadcaster {
public:
    MainComponent();

    ~MainComponent() = default;

    void paint(juce::Graphics& g) override;

    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster* source);

    void actionListenerCallback(const juce::String& message);

    void playTrack(TrackInfo track);

private:
    Transport transport;
    TransportComponent transportComponent;
    FileProcessor fileProcessor;
    int transportPadding = 10;
    BrowserComponent browser;
    InMemLibrary library;
    PlayQueue playQueue;
    TopBarComponent topBar;
    std::unordered_map<juce::String, std::function<void()>> actionHandlers;
    // =================================
    void configureTransport();
    void configureTopBar();
    void configureBrowser();
    void configureElements();
    // =================================
    void resizeBrowser();
    void resizeTransport();
    void resizeTopBar();
    // =================================
    void handleTracksAdded();
    void handleTransportChange();
    void handleLoadSelectedMessage();
    void handleQueueMessage();
    void handlePauseMessage();
    void handlePlayMessage();
    void handleStopMessage();
    void handleViewLibraryMessage();
    void handleViewPlayQueueMessage();
    void handleNextTrackMessage();
    void handleRestartTrackMessage();
    void handlePlayAlbumMessage();
    void handlePlayArtistMessage();
    void handleRemoveFromLibraryMessage();
    void handleDeleteFromHarddriveMessage();
    void handleShuffleModeChangedMessage();
    void handlePlayRandomAlbumMessage();
    void configureActionHandlers();
    // =================================
    void playNextTrack();
    void overwritePlayQueue(std::vector<TrackInfo> tracks, std::string sortAttribute);
    TrackInfo getRandomTrackToPlay();
    std::vector<TrackInfo> getRandomAlbumToPlay();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
