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

TransportState Transport::getState() { return state; }

void Transport::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void Transport::releaseResources() { transportSource.releaseResources(); }

void Transport::setGain(float gain) { transportSource.setGain(gain); }

bool Transport::hasPlayableSource() { return readerSource.get() != nullptr; }

const TrackInfo& Transport::getCurrentTrack() { return currentTrack; }

bool Transport::canStart() { return hasPlayableSource() && !isPlaying(); }

bool Transport::canStop() { return isPlaying() || isPaused(); }

bool Transport::canPause() { return isPlaying(); }

bool Transport::isPlaying() { return getState() == PLAYING; }

bool Transport::isPaused() { return getState() == PAUSED; }

bool Transport::isStopped() { return getState() == STOPPED; }

juce::String Transport::getWildcardForAllFormats() {
    return formatManager.getWildcardForAllFormats();
}

bool Transport::trackFinished() { return transportSource.hasStreamFinished(); }

bool Transport::hasActiveTrack() { return _hasActiveTrack; }

double Transport::getCurrentPosition() { return transportSource.getCurrentPosition(); }

void Transport::setCurrentTrack(TrackInfo track) { currentTrack = track; }

void Transport::start() {
    if (!canStart())
        return;
    transportSource.start();
    setState(STARTING);
}

void Transport::stop() {
    if (!canStop())
        return;
    setPosition(0.0);
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

void Transport::setPosition(double position) { transportSource.setPosition(position); }

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
        if (transportSource.isPlaying()) {
            setState(TransportState::PLAYING);
        } else if (getState() == TransportState::PAUSING) {
            setState(TransportState::PAUSED);
        } else {
            setState(TransportState::STOPPED);
            _hasActiveTrack = !trackFinished();
        }
    }
}

void Transport::setCurrentTrack(juce::File file) {
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

bool Transport::loadTrack(juce::File file) {
    auto* reader = formatManager.createReaderFor(file);
    if (reader == nullptr)
        return false;
    auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
    setCurrentTrack(file);
    currentTrack.setMetadata(reader->metadataValues);
    readerSource.reset(newSource.release());
    setState(TransportState::READY);
    _hasActiveTrack = true;
    return true;
}

bool Transport::loadTrack(TrackInfo track) {
    auto* reader = formatManager.createReaderFor(track.getPath());
    if (reader == nullptr)
        return false;
    auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
    readerSource.reset(newSource.release());
    setCurrentTrack(track);
    setState(TransportState::READY);
    _hasActiveTrack = true;
    return true;
}
