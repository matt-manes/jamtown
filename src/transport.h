#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>
#include <memory>
#include <string>
#include <unordered_map>
#include "trackInfo.h"

enum TransportState : int {
    STOPPED,
    STARTING,
    PLAYING,
    STOPPING,
    PAUSING,
    PAUSED,
    READY
};

class Transport : public juce::AudioSource,
                  public juce::ChangeListener,
                  public juce::ChangeBroadcaster {
 public:
    Transport();

    TransportState getState() { return state; }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override {
        transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void releaseResources() override { transportSource.releaseResources(); };

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void setGain(float gain) { transportSource.setGain(gain); }

    bool hasPlayableSource() { return readerSource.get() != nullptr; }

    bool setSource(juce::File);

    const TrackInfo& getCurrentTrack() { return currentTrack; }

    void start();

    void stop();

    void pause();

    bool canStart() { return hasPlayableSource() && !isPlaying(); }

    bool canStop() { return isPlaying() || isPaused(); }

    bool canPause() { return isPlaying(); }

    bool isPlaying() { return getState() == PLAYING; }

    bool isPaused() { return getState() == PAUSED; }

    bool isStopped() { return getState() == STOPPED; }

 private:
    TransportState state;
    TrackInfo currentTrack;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    void setTrackInfo(juce::File);

    void setState(TransportState newState);
};
