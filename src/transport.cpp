#include "transport.h"
#include <juce_audio_utils/juce_audio_utils.h>
#include <memory>
#include <string>
#include <unordered_map>

Transport::Transport() {
    transportSource.addChangeListener(this);
    formatManager.registerBasicFormats();
    setState(STOPPED);
}

void Transport::start() {
    if (!canStart())
        return;
    transportSource.start();
    setState(STARTING);
}

void Transport::stop() {
    if (!canStop())
        return;
    transportSource.setPosition(0.0);
    // transportSource.stop() doesn't broadcast a state change
    // message if it isn't actually playing
    // If we don't differentiate here, transport gets stuck in STOPPING
    // when stopping while paused
    if (isPlaying()) {
        transportSource.stop();
        setState(STOPPING);
    } else if (isPaused()) {
        setState(STOPPED);
    }
}

void Transport::pause() {
    if (!canPause())
        return;
    transportSource.stop();
    setState(PAUSING);
}

void Transport::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) {
    if (!hasPlayableSource()) {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    transportSource.getNextAudioBlock(bufferToFill);
}

void Transport::setState(TransportState newState) {
    if (newState != getState()) {
        state = newState;
        sendChangeMessage();
    }
}

void Transport::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &transportSource) {
        if (transportSource.isPlaying())
            setState(TransportState::PLAYING);
        else if (getState() == TransportState::PAUSING)
            setState(TransportState::PAUSED);
        else
            setState(TransportState::STOPPED);
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
    setState(TransportState::READY);
    return true;
}
