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
    TransportState getState();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void releaseResources() override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    /**
     * @brief Set the playback gain.
     *
     * @param gain The gain to apply, 1.0 is unity.
     */
    void setGain(float gain);

    /**
     * @brief Whether a playable track has been loaded.
     *
     * @return true
     * @return false
     */
    bool hasPlayableSource();

    /**
     * @brief Load the given file for playback and return whether it was successful.
     *
     * @return true
     * @return false
     */
    bool loadTrack(juce::File);

    /**
     * @brief Load the given track for playback, returning whether loading was successful or not.
     *
     * @param track
     * @return true
     * @return false
     */
    bool loadTrack(TrackInfo track);

    /**
     * @brief Get the currently loaded track.
     *
     * @return const TrackInfo&
     */
    const TrackInfo& getCurrentTrack();

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
    bool canStart();

    /**
     * @brief Whether playback can be stopped or not.
     *
     * @return true
     * @return false
     */
    bool canStop();

    /**
     * @brief Whether playback can be paused or not.
     *
     * @return true
     * @return false
     */
    bool canPause();

    /**
     * @brief Whether the transport is currently playing or not.
     *
     * @return true
     * @return false
     */
    bool isPlaying();

    /**
     * @brief Whether the transport is paused.
     *
     * @return true
     * @return false
     */
    bool isPaused();

    /**
     * @brief Whether the transport is stopped.
     *
     * @return true
     * @return false
     */
    bool isStopped();

    /**
     * @brief Get a wildcard string for the file types playable by this transport object.
     *
     * @return juce::String
     */
    juce::String getWildcardForAllFormats();

    /**
     * @brief Whether the current track has finished playing.
     *
     * @return true
     * @return false
     */
    bool trackFinished();

    /**
     * @brief Whether a track has been loaded for playback.
     *
     * @return true
     * @return false
     */
    bool hasActiveTrack();

    /**
     * @brief Get the current playback position in seconds.
     *
     * @return double
     */
    double getCurrentPosition();

    /**
     * @brief Set the current playback position in seconds.
     *
     * @param position
     */
    void setPosition(double position);

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
    void setCurrentTrack(juce::File);

    /**
     * @brief Set `currentTrack` to the given track.
     *
     * @param track
     */
    void setCurrentTrack(TrackInfo track);

    /**
     * @brief Update the transport state tracker and send a notification to any listeners.
     *
     * @param newState
     */
    void setState(TransportState newState);
};
