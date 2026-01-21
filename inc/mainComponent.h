#pragma once

#include "transportComponent.h"
#include "addTracksComponent.h"
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>
#include <unordered_map>
#include "playQueue.h"
#include "fileProcessor.h"
#include "inMemLibrary.h"
#include "browserComponent.h"
#include "transport.h"
#include "topBarComponent.h"
#include "searchService.h"
#include "searchBoxComponent.h"

class MainComponent : public juce::Component,
                      public juce::ChangeListener,
                      public juce::ActionListener,
                      public juce::ActionBroadcaster {
public:
    MainComponent();

    ~MainComponent() {
        // Prevent leakage
        while (loadingLib && !libLoaded)
            juce::Thread::sleep(1);
    };

    void paint(juce::Graphics& g) override;

    void resized() override;

    void changeListenerCallback(juce::ChangeBroadcaster* source);

    void actionListenerCallback(const juce::String& message);

    /**
     * @brief Play the given track.
     *
     * @param track
     */
    void playTrack(TrackInfo track);

    /**
     * @brief Load library from disk.
     *
     */
    void loadLibrary();

private:
    Transport transport;
    TransportComponent transportComponent;
    FileProcessor fileProcessor;
    int transportPadding = 10;
    BrowserComponent browser;
    InMemLibrary library;
    PlayQueue playQueue;
    TopBarComponent topBar;
    SearchService searchService;
    SearchBoxComponent searchBox;
    std::atomic<bool> libLoaded = false;
    std::atomic<bool> loadingLib = false;
    std::unordered_map<juce::String, std::function<void()>> actionHandlers;
    // =================================
    void configureTransport();
    void configureTopBar();
    void configureBrowser();
    void configureSearchBox();
    void configureElements();
    // =================================
    void resizeBrowser();
    void resizeTransport();
    void resizeTopBar();
    void resizeSearchBox();
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
    void handleRemoveSelectedFromPlayQueueMessage();
    void handleSearchUpdatedMessage();
    void handleLibraryUpdatedMessage();
    void configureActionHandlers();
    // =================================
    void playNextTrack();
    void overwritePlayQueue(std::vector<TrackInfo> tracks, std::string sortAttribute);
    TrackInfo getRandomTrackToPlay();
    std::vector<TrackInfo> getRandomAlbumToPlay();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
