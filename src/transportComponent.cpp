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

void TransportComponent::configurePlayButton() {
    playButton.onClick = [this] { playButtonClicked(); };
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::turquoise);
    playButton.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    playButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    playButton.setButtonText("Play");
}

void TransportComponent::configureStopButton() {
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::deeppink);
    stopButton.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    stopButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
}

void TransportComponent::configureInterface() {
    addAndMakeVisible(&playButton);
    configurePlayButton();

    addAndMakeVisible(&stopButton);
    configureStopButton();

    addAndMakeVisible(&display);
    display.setColour(juce::Label::textColourId, juce::Colours::hotpink);
    // setDisplayText("??????");

    // addAndMakeVisible(&stateLabel);
}

void TransportComponent::resized() {
    // TODO read these values from a file on start up
    int halfWidth = getWidth() / 2;
    stopButton.setSize(halfWidth - 10, 20);
    stopButton.setTopLeftPosition(0, getHeight() - stopButton.getHeight());
    playButton.setSize(halfWidth - 10, 20);
    playButton.setTopLeftPosition(halfWidth + 10, getHeight() - playButton.getHeight());
    auto font = display.getFont();
    int displayHeight = static_cast<int>(font.getHeight() * getDisplayLineCount());
    display.setSize(getWidth(), displayHeight);
    display.setTopLeftPosition(0, stopButton.getY() - displayHeight);
    //  stateLabel.setBounds(10, 130, getWidth() - 20, 20);
}

void TransportComponent::paint(juce::Graphics& g) {
    g.setColour(juce::Colours::black);
    g.fillAll();
}

void TransportComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &transport) {
        updateUI();
        if (transport.trackFinished())
            sendChangeMessage();
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
    setDisplayText(transport.getCurrentTrack().toString());
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
    setDisplayText(transport.getCurrentTrack().toString());
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

int TransportComponent::getDisplayLineCount() {
    auto text = display.getText();
    int count = 1;
    int pos = -1;
    while ((pos = text.indexOf(pos + 1, "\n")) != -1) {
        ++count;
    }
    return count;
}

void TransportComponent::setDisplayText(std::string text) {
    display.setText(text, {});
    resized();
}
