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
class TransportComponent : public juce::AudioAppComponent, public juce::ChangeListener {
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

 private:
    void updateUI();

    // TODO repurpose this for selecting folder to sync to library
    void openButtonClicked();
    void playButtonClicked();
    void stopButtonClicked();

    void stoppedHandler();
    void startingHandler();
    void playingHandler();
    void pausedHandler();
    void readyHandler();

    void configureHandlers();

    void configureOpenButton();
    void configurePlayButton();
    void configureStopButton();

    void configureInterface();

    void startPlayback();
    void pausePlayback();
    void stopPlayback();

    //==========================================================================
    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::Label display;
    juce::Label stateLabel;

    std::unique_ptr<juce::FileChooser> chooser;

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
