#include "transport.h"
#include <juce_audio_utils/juce_audio_utils.h>
#include <memory>
#include <string>
#include <unordered_map>

Transport::Transport() {
    transportSource.addChangeListener(this);
    configureHandlers();
    formatManager.registerBasicFormats();
    changeState(STOPPED);
}

void Transport::stoppedHandler() { transportSource.setPosition(0.0); }

void Transport::startingHandler() { transportSource.start(); }

void Transport::stoppingHandler() {
    if (transportSource.isPlaying())
        transportSource.stop();
    else
        changeState(STOPPED);
}

void Transport::pausingHandler() { transportSource.stop(); }

void Transport::configureHandlers() {
    handlers.emplace(STOPPED, [this] { stoppedHandler(); });
    handlers.emplace(STARTING, [this] { startingHandler(); });
    handlers.emplace(STOPPING, [this] { stoppingHandler(); });
    handlers.emplace(PAUSING, [this] { pausingHandler(); });
}

void Transport::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    if (!hasPlayableSource()) {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    transportSource.getNextAudioBlock(bufferToFill);
}

void Transport::changeState(TransportState newState) {
    if (newState != getState()) {
        currentState = newState;
        // may not need to do anything for a given state
        if (handlers.contains(currentState))
            handlers[currentState]();
        // but somebody else may so send change message
        sendChangeMessage();
    }
}

void Transport::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &transportSource) {
        if (transportSource.isPlaying())
            changeState(TransportState::PLAYING);
        else if (getState() == TransportState::PAUSING)
            changeState(TransportState::PAUSED);
        else
            changeState(TransportState::STOPPED);
    }
}

void Transport::setTrackInfo(juce::File file) {
    // TODO try to read metadata then fallback on path parsing if that fails
    std::string artist =
        file.getParentDirectory().getParentDirectory().getFileName().toStdString();
    std::string album = file.getParentDirectory().getFileName().toStdString();
    std::string title = file.getFileNameWithoutExtension().toStdString();
    // Assumes this is called after `transportSource.setSource`
    // Otherwise `length` will be the length of the previous track
    double length = transportSource.getLengthInSeconds();
    currentTrack = TrackInfo{artist, album, title, length, file};
}

bool Transport::setSource(juce::File file) {
    auto* reader = formatManager.createReaderFor(file);
    if (reader == nullptr)
        return false;
    auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
    setTrackInfo(file);
    currentTrack.setMetadata(reader->metadataValues);
    readerSource.reset(newSource.release());
    changeState(TransportState::READY);
    return true;
}
