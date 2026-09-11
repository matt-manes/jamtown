#pragma once

#include <memory>
#include <unordered_map>
#include <juce_audio_utils/juce_audio_utils.h>
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
#include "tracksAddedListener.h"
#include "libraryUpdatedListener.h"
#include "searchUpdatedListener.h"
#include "removeFromLibraryListener.h"
#include "transportStateChangeListener.h"
#include "trackController.h"
#include "newTrackSelectedListener.h"
#include "playQueueUpdatedListener.h"
#include "loadSelectedTracksListener.h"
#include "queueTrackListener.h"
#include "dequeueTrackListener.h"
#include "playRandomAlbumListener.h"
#include "deleteFromHarddriveListener.h"
#include "playArtistListener.h"
#include "playAlbumListener.h"
#include "transportCommandListener.h"
#include "changeBrowserViewListener.h"

class MainComponent : public juce::Component {
public:
    MainComponent();

    ~MainComponent() {
        // Prevent leakage
        while (loadingLib && !libLoaded)
            juce::Thread::sleep(1);
    };

    void paint(juce::Graphics& g) override;

    void resized() override;

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
    std::unique_ptr<Library> library;
    PlayQueue playQueue;
    SearchService searchService;
    TopBarComponent topBar;
    TrackController trackController;
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
    //  =================================
    TracksAddedListener tracksAddedListener;
    LibraryUpdatedListener libraryUpdatedListener;
    SearchUpdatedListener searchUpdatedListener;
    RemoveFromLibraryListener removeFromLibraryListener;
    TransportStateChangeListener transportStateChangeListener;
    NewTrackSelectedListener newTrackSelectedListener;
    PlayQueueUpdatedListener playQueueUpdatedListener;
    LoadSelectedTracksListener loadSelectedTracksListener;
    QueueTrackActionListener queueTrackActionListener;
    DequeueTrackListener dequeueTrackActionListener;
    PlayRandomAlbumListener playRandomAlbumActionListener;
    DeleteFromHarddriveListener deleteFromHarddriveListener;
    PlayArtistListener playArtistListener;
    PlayAlbumListener playAlbumListener;
    TransportCommandListener transportCommandListener;
    ChangeBrowserViewListener changeBrowserViewListener;
    //  =================================
    void configureTransport();
    void configureTopBar();
    void configureBrowser();
    void configureLayout();
    void configureListeners();
    void initializeComponent();
    // =================================

    void overwritePlayQueue(std::vector<TrackInfo> tracks, std::string sortAttribute);
    TrackInfo getRandomTrackToPlay();
    std::vector<TrackInfo> getRandomAlbumToPlay();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
