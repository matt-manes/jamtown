#include "mainComponent.h"
#include <juce_gui_basics/juce_gui_basics.h>
#include <algorithm>
#include <vector>
#include <string>
#include <memory>
#include "actionMessages.h"

MainComponent::MainComponent()
    : transportComponent(&transportController), topBar(&searchService),
      libraryPersistanceService(std::make_unique<TxtLibraryPersistanceService>()),
      library(std::make_unique<InMemLibrary>()),
      tracksAddedListener(topBar,
                          *library.get(),
                          *libraryPersistanceService.get(),
                          searchService,
                          browser),
      libraryUpdatedListener(
          *library.get(), *libraryPersistanceService.get(), browser, searchService),
      searchUpdatedListener(*library.get(), browser, searchService),
      removeFromLibraryListener(*library.get(), browser, searchService),
      trackController(
          *library.get(), playQueue, browser, transportController, transportComponent),
      transportStateChangeListener(trackController, transportController),
      newTrackSelectedListener(browser, transportController),
      playQueueUpdatedListener(browser, playQueue),
      loadSelectedTracksListener(browser, transportController, playQueue),
      queueTrackActionListener(playQueue, browser, transportController, trackController),
      dequeueTrackActionListener(playQueue, browser),
      playRandomAlbumActionListener(trackController),
      deleteFromHarddriveListener(browser, *library.get(), searchService),
      playArtistListener(*library.get(), trackController, browser),
      playAlbumListener(*library.get(), trackController, browser),
      transportCommandListener(trackController, transportController),
      changeBrowserViewListener(browser) {
    initializeComponent();
}

void MainComponent::loadLibrary() {
    // Using `loadingLib` and `libLoaded`
    // to prevent leaks if user exits
    // while library is being loaded
    loadingLib = true;
    juce::Thread::launch([this]() {
        auto tracks = libraryPersistanceService->load();
        searchService.addTracks(tracks);
        library->addTracks(tracks);
        libLoaded = true;
    });
}

void MainComponent::configureTransport() { transportController.stop(); }

void MainComponent::configureTopBar() {
    topBar.setTrackAdderWildcard(transportController.getWildcardForAllFormats());
}

void MainComponent::configureBrowser() {
    browser.setLibrary(library.get());
    browser.setPlayQueue(&playQueue);
}

void MainComponent::configureLayout() {
    layoutBox.flexDirection = juce::FlexBox::Direction::column;
    layoutBox.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    layoutBox.items.add(
        juce::FlexItem(topBar).withFlex(0, 1, topBarHeight).withMargin(columnItemMargin));
    layoutBox.items.add(juce::FlexItem(browser).withFlex(1).withMargin(columnItemMargin));
    layoutBox.items.add(juce::FlexItem(transportComponent)
                            .withFlex(0, 1, transportComponentHeight)
                            .withMargin(bottomMargin));
}

void MainComponent::configureListeners() {
    topBar.addTrackAdderListener(&tracksAddedListener);
    topBar.addSearchBoxChangeListener(&searchUpdatedListener);
    topBar.addFunctionButtonsActionListener(&changeBrowserViewListener);

    library->addChangeListener(&libraryUpdatedListener);

    trackController.addChangeListener(&newTrackSelectedListener);

    transportController.addChangeListener(&transportStateChangeListener);
    transportController.addChangeListener(&transportComponent);

    playQueue.addChangeListener(&playQueueUpdatedListener);

    browser.libraryView->addActionListener(&removeFromLibraryListener);
    browser.libraryView->addActionListener(&loadSelectedTracksListener);
    browser.libraryView->addActionListener(&queueTrackActionListener);
    browser.libraryView->addActionListener(&playArtistListener);
    browser.libraryView->addActionListener(&playAlbumListener);

    browser.playQueueView->addActionListener(&loadSelectedTracksListener);
    browser.playQueueView->addActionListener(&dequeueTrackActionListener);

    transportComponent.addActionListener(&playRandomAlbumActionListener);
    transportComponent.addActionListener(&transportCommandListener);
}

void MainComponent::initializeComponent() {
    configureTransport();
    addAndMakeVisible(transportComponent);

    configureBrowser();
    addAndMakeVisible(browser);

    configureTopBar();
    addAndMakeVisible(topBar);

    configureListeners();

    configureLayout();
    loadLibrary();
}

void MainComponent::paint(juce::Graphics& g) {
    g.setColour(juce::Colours::black);
    g.fillAll();
}

void MainComponent::resized() { layoutBox.performLayout(getLocalBounds()); }
