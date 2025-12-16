#include <juce_graphics/juce_graphics.h>
#include <memory>
#include "transportComponent.h"

TransportComponent::TransportComponent() {
    configureInterface();
    configureHandlers();
    transport.addChangeListener(this);
    setAudioChannels(0, 2);
    // normally called by the listener callback
    // but here transport state is already set so make manual call
    updateUI();
}

void TransportComponent::configureOpenButton() {
    openButton.setButtonText("Open...");
    openButton.onClick = [this] { openButtonClicked(); };
}

void TransportComponent::configurePlayButton() {
    playButton.setColour(juce::TextButton::textColourOnId, juce::Colours::magenta);
    playButton.setColour(juce::TextButton::textColourOffId, juce::Colours::magenta);
    playButton.onClick = [this] { playButtonClicked(); };
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::green);
    playButton.setButtonText("Play");
}

void TransportComponent::configureStopButton() {
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::red);
}

void TransportComponent::configureInterface() {
    addAndMakeVisible(&openButton);
    configureOpenButton();

    addAndMakeVisible(&playButton);
    configurePlayButton();

    addAndMakeVisible(&stopButton);
    configureStopButton();

    addAndMakeVisible(&display);
    display.setText("??????", {});

    addAndMakeVisible(&stateLabel);
}

void TransportComponent::resized() {
    // TODO read these values from a file on start up
    openButton.setBounds(10, 10, getWidth() - 20, 20);
    playButton.setBounds(10, 40, getWidth() - 20, 20);
    stopButton.setBounds(10, 70, getWidth() - 20, 20);
    display.setBounds(10, 100, getWidth() - 20, getBottom() - 130);
    stateLabel.setBounds(10, 130, getWidth() - 20, 20);
}

void TransportComponent::paint(juce::Graphics& g) {
    g.setColour(juce::Colours::black);
    g.fillAll();
}

void TransportComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &transport) {
        updateUI();
    }
}

void TransportComponent::updateUI() {
    if (stateChangeHandlers.contains(transport.getState()))
        stateChangeHandlers[transport.getState()]();
    stateLabel.setText("State: " + getStateString(), {});
}

void TransportComponent::playButtonClicked() {
    if (transport.isPlaying()) {
        pausePlayback();
    } else {
        startPlayback();
    }
}

void TransportComponent::stopButtonClicked() { stopPlayback(); }

void TransportComponent::openButtonClicked() {
    chooser = std::make_unique<juce::FileChooser>("Select a Wave file to play...",
                                                  juce::File{});
    auto chooserFlags =
        juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc) {
        auto file = fc.getResult();
        if (file == juce::File{})
            return;
        if (transport.setSource(file)) {
            display.setText((transport.getCurrentTrack().toString()), {});
        } else {
            juce::NativeMessageBox::showMessageBoxAsync(
                juce::MessageBoxIconType::WarningIcon, "Error", "Unable to read file.");
        }
    });
}

void TransportComponent::stoppedHandler() {
    stopButton.setEnabled(false);
    playButton.setButtonText("Play");
    playButton.setEnabled(transport.hasPlayableSource() ? true : false);
}

void TransportComponent::startingHandler() { playButton.setEnabled(false); }

void TransportComponent::playingHandler() {
    stopButton.setEnabled(true);
    playButton.setButtonText("Pause");
    playButton.setEnabled(true);
}

void TransportComponent::pausedHandler() {
    playButton.setButtonText("Play");
    playButton.setEnabled(true);
    stopButton.setEnabled(true);
}

void TransportComponent::readyHandler() {
    playButton.setButtonText("Play");
    playButton.setEnabled(true);
    stopButton.setEnabled(false);
}

void TransportComponent::configureHandlers() {
    stateChangeHandlers.emplace(TransportState::STOPPED, [this] { stoppedHandler(); });
    stateChangeHandlers.emplace(TransportState::STARTING, [this] { startingHandler(); });
    stateChangeHandlers.emplace(TransportState::PLAYING, [this] { playingHandler(); });
    stateChangeHandlers.emplace(TransportState::PAUSED, [this] { pausedHandler(); });
    stateChangeHandlers.emplace(TransportState::READY, [this] { readyHandler(); });
}

void TransportComponent::startPlayback() { transport.start(); }

void TransportComponent::pausePlayback() { transport.pause(); }

void TransportComponent::stopPlayback() { transport.stop(); }
