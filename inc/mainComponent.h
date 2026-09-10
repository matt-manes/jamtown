#pragma once

#include <memory>
#include <unordered_map>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_events/juce_events.h>
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include "transportComponent.h"
#include "addTracksComponent.h"
#include "playQueue.h"
#include "fileProcessor.h"
#include "inMemLibrary.h"
#include "browserComponent.h"
#include "transportController.h"
#include "functionButtonsComponent.h"
#include "searchService.h"
#include "searchBoxComponent.h"
#include "libraryPersistanceService.h"
#include "txtLibraryPersistanceService.h"
#include "topBarComponent.h"

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
    TransportController transportController;
    TransportComponent transportComponent;
    FileProcessor fileProcessor;
    BrowserComponent browser;
    InMemLibrary library;
    PlayQueue playQueue;
    SearchService searchService;
    TopBarComponent topBar;
    float topBarHeight = 20;
    float transportComponentHeight = 60;
    float columnItemSpacing = 2;
    float bottomSpacing = 10;
    juce::FlexItem::Margin columnItemMargin{0, 0, columnItemSpacing, 0};
    juce::FlexItem::Margin bottomMargin{0, 0, bottomSpacing, 0};
    juce::FlexBox layoutBox;
    std::unique_ptr<LibraryPersistanceService> libraryPersistanceService;
    std::atomic<bool> libLoaded = false;
    std::atomic<bool> loadingLib = false;
    std::unordered_map<juce::String, std::function<void()>> actionHandlers;
    // =================================
    void configureTransport();
    void configureTopBar();
    void configureBrowser();
    void configureLayout();
    void initializeComponent();
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
