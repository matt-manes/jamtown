#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_events/juce_events.h>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include "transport.h"
#include "timeFormatter.h"

/**
 * @brief The transport UI component.
 *
 */
class TransportComponent : public juce::AudioAppComponent,
                           public juce::ChangeListener,
                           public juce::ChangeBroadcaster,
                           public juce::ActionBroadcaster {
public:
    /**
     * @brief Construct a new Transport Component object
     *
     */
    TransportComponent(Transport* transport);

    /**
     * @brief Destroy the Transport Component object
     *
     */
    ~TransportComponent() override { shutdownAudio(); }

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override {
        transport->prepareToPlay(samplesPerBlockExpected, sampleRate);
    }

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override {
        transport->getNextAudioBlock(bufferToFill);
    }

    void releaseResources() override { transport->releaseResources(); }

    void resized() override;

    void paint(juce::Graphics& g) override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

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
    void configureElapsedTimeLabel();

    /**
     * @brief Any UI element setup functions should be called here.
     *
     */
    void configureInterface();

    int getDisplayLineCount();
    void setDisplayText(std::string text);

    class ElapsedTime : public juce::AnimatedAppComponent {
    public:
        ElapsedTime(Transport* transport) : transport(transport) {
            label.setColour(juce::Label::textColourId, juce::Colours::hotpink);
            addAndMakeVisible(label);
            setFramesPerSecond(5);
        }

        ~ElapsedTime() = default;

        void resized() override { label.setBounds(0, 0, getWidth(), getHeight()); }

        void update() override {
            if (transport->hasActiveTrack()) {
                label.setText(formatSeconds(transport->getCurrentPosition()), {});
            } else {
                label.setText("", {});
            }
        }

        void paint(juce::Graphics& g) override {
            g.setColour(juce::Colours::black);
            g.fillAll();
        }
        Transport* transport;
        juce::Label label;
    };

    //==========================================================================
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::Label currentTrackInfo;
    juce::Label stateLabel;
    ElapsedTime elapsedTime;
    Transport* transport;

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
