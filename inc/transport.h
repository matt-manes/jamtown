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

    /**
     * @brief Get the current transport state.
     *
     * @return TransportState
     */
    TransportState getState() { return state; }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override {
        transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void releaseResources() override { transportSource.releaseResources(); };

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    /**
     * @brief Set the playback gain.
     *
     * @param gain The gain to apply, 1.0 is unity.
     */
    void setGain(float gain) { transportSource.setGain(gain); }

    /**
     * @brief Whether a playable track has been loaded.
     *
     * @return true
     * @return false
     */
    bool hasPlayableSource() { return readerSource.get() != nullptr; }

    /**
     * @brief Load the given file for playback and return whether it was successful.
     *
     * @return true
     * @return false
     */
    bool loadTrack(juce::File);

    bool loadTrack(TrackInfo track);

    /**
     * @brief Get the currently loaded track.
     *
     * @return const TrackInfo&
     */
    const TrackInfo& getCurrentTrack() { return currentTrack; }

    /**
     * @brief Start playback if able.
     *
     */
    void start();

    /**
     * @brief Stop playback if able.
     *
     */
    void stop();

    /**
     * @brief Pause playback if able.
     *
     */
    void pause();

    /**
     * @brief Whether playback can be started or not.
     *
     * @return true
     * @return false
     */
    bool canStart() { return hasPlayableSource() && !isPlaying(); }

    /**
     * @brief Whether playback can be stopped or not.
     *
     * @return true
     * @return false
     */
    bool canStop() { return isPlaying() || isPaused(); }

    /**
     * @brief Whether playback can be paused or not.
     *
     * @return true
     * @return false
     */
    bool canPause() { return isPlaying(); }

    bool isPlaying() { return getState() == PLAYING; }

    bool isPaused() { return getState() == PAUSED; }

    bool isStopped() { return getState() == STOPPED; }

    juce::String getWildcardForAllFormats() {
        return formatManager.getWildcardForAllFormats();
    }

    bool trackFinished() { return transportSource.hasStreamFinished(); }

    bool hasActiveTrack() { return _hasActiveTrack; }

private:
    TransportState state;
    TrackInfo currentTrack;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    bool _hasActiveTrack = false;

    /**
     * @brief Populate `currentTrack` using the given file.
     *
     */
    void setTrackInfo(juce::File);

    void setTrackInfo(TrackInfo track) { currentTrack = track; }

    /**
     * @brief Update the transport state tracker and send a notification to any listeners.
     *
     * @param newState
     */
    void setState(TransportState newState);
};
