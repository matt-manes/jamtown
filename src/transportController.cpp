#include "transportController.h"
#include <juce_audio_utils/juce_audio_utils.h>
#include <memory>
#include <string>
#include <unordered_map>

TransportController::TransportController() {
    transportSource.addChangeListener(this);
    formatManager.registerBasicFormats();
    setState(STOPPED);
}

TransportState TransportController::getState() { return state; }

void TransportController::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void TransportController::releaseResources() { transportSource.releaseResources(); }

void TransportController::setGain(float gain) { transportSource.setGain(gain); }

bool TransportController::hasPlayableSource() { return readerSource.get() != nullptr; }

const TrackInfo& TransportController::getCurrentTrack() { return currentTrack; }

bool TransportController::canStart() { return hasPlayableSource() && !isPlaying(); }

bool TransportController::canStop() { return isPlaying() || isPaused(); }

bool TransportController::canPause() { return isPlaying(); }

bool TransportController::isPlaying() { return getState() == PLAYING; }

bool TransportController::isPaused() { return getState() == PAUSED; }

bool TransportController::isStopped() { return getState() == STOPPED; }

juce::String TransportController::getWildcardForAllFormats() {
    return formatManager.getWildcardForAllFormats();
}

bool TransportController::trackFinished() { return transportSource.hasStreamFinished(); }

bool TransportController::hasActiveTrack() { return _hasActiveTrack; }

double TransportController::getCurrentPosition() {
    return transportSource.getCurrentPosition();
}

void TransportController::setCurrentTrack(TrackInfo track) { currentTrack = track; }

void TransportController::start() {
    if (!canStart())
        return;
    transportSource.start();
    setState(STARTING);
}

void TransportController::stop() {
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

void TransportController::pause() {
    if (!canPause())
        return;
    transportSource.stop();
    setState(PAUSING);
}

void TransportController::setPosition(double position) {
    transportSource.setPosition(position);
}

void TransportController::getNextAudioBlock(
    const juce::AudioSourceChannelInfo& bufferToFill) {
    if (!hasPlayableSource()) {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    transportSource.getNextAudioBlock(bufferToFill);
}

void TransportController::setState(TransportState newState) {
    if (newState != getState()) {
        state = newState;
        sendChangeMessage();
    }
}

void TransportController::changeListenerCallback(juce::ChangeBroadcaster* source) {
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

void TransportController::setCurrentTrack(juce::File file) {
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

bool TransportController::loadTrack(juce::File file) {
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

bool TransportController::loadTrack(TrackInfo track) {
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
