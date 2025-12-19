#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_events/juce_events.h>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include "transport.h"

/**
 * @brief The transport UI component.
 *
 */
class TransportComponent : public juce::AudioAppComponent,
                           public juce::ChangeListener,
                           public juce::ChangeBroadcaster {
public:
    /**
     * @brief Construct a new Transport Component object
     *
     */
    TransportComponent();

    /**
     * @brief Destroy the Transport Component object
     *
     */
    ~TransportComponent() override { shutdownAudio(); }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override {
        transport.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override {
        transport.getNextAudioBlock(bufferToFill);
    }

    void releaseResources() override { transport.releaseResources(); }

    void resized() override;

    void paint(juce::Graphics& g) override;

    /**
     * @brief Set the playback volume.
     *
     * @param gain The gain to apply, 1.0 is unity.
     */
    void setGain(float gain) { transport.setGain(gain); }

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    /**
     * @brief Convert transport state enum into a string.
     *
     * @return std::string The current transport state as a string.
     */
    std::string getStateString() { return stateMap[transport.getState()]; }

    juce::String getWildcardForAllFormats() {
        return transport.getWildcardForAllFormats();
    }

    void loadTrack(TrackInfo track) { transport.loadTrack(track); }

    void startPlayback();
    void pausePlayback();
    void stopPlayback();

    bool trackFinished() { return transport.trackFinished(); }

    bool hasActiveTrack() { return transport.hasActiveTrack(); }

private:
    /**
     * @brief Update GUI elements.
     * Typically called from `changeListenerCallback()`.
     *
     */
    void updateUI();

    /**
     * @brief Callback for handling play button click.
     *
     */
    void playButtonClicked();

    /**
     * @brief Callback for handling stop button click.
     *
     */
    void stopButtonClicked();

    /*
    None of these '*Handler' functions are called directly.
    They are stored in the `stateChangeHandlers` map and are
    called by `updateUI()`.
    */

    /**
     * @brief Update UI for STOPPED state.
     *
     */
    void stoppedHandler();

    /**
     * @brief Update UI for STARTING state.
     *
     */
    void startingHandler();

    /**
     * @brief Update UI for PLAYING state.
     *
     */
    void playingHandler();

    /**
     * @brief Update UI for PAUSED state.
     *
     */
    void pausedHandler();

    /**
     * @brief Update UI for READY state.
     *
     */
    void readyHandler();

    /**
     * @brief Maps `TransportState` enums to handler functions.
     *
     */
    void configureHandlers();

    void configurePlayButton();
    void configureStopButton();

    /**
     * @brief Any UI element setup functions should be called here.
     *
     */
    void configureInterface();

    int getDisplayLineCount();
    void setDisplayText(std::string text);

    //==========================================================================
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::Label display;
    juce::Label stateLabel;

    Transport transport;

    std::unordered_map<TransportState, std::function<void()>> stateChangeHandlers;
    std::unordered_map<TransportState, std::string> stateMap = {
        {TransportState::STOPPED, "Stopped"},
        {TransportState::STARTING, "Starting"},
        {TransportState::PLAYING, "Playing"},
        {TransportState::STOPPING, "Stopping"},
        {TransportState::PAUSING, "Pausing"},
        {TransportState::PAUSED, "Paused"},
        {TransportState::READY, "Ready"}};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TransportComponent)
};
