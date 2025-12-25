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
                           public juce::ActionBroadcaster,
                           public juce::Slider::Listener {
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

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;

    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;

    void releaseResources() override;

    void resized() override;

    void paint(juce::Graphics& g) override;

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void sliderValueChanged(juce::Slider* slider) override;

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
    void configureVolumeSlider();

    /**
     * @brief Any UI element setup functions should be called here.
     *
     */
    void configureInterface();

    int getDisplayLineCount();
    void setDisplayText(std::string text);

    class ElapsedTime : public juce::AnimatedAppComponent {
    public:
        ElapsedTime(Transport* transport);

        ~ElapsedTime() = default;

        void resized() override;

        void update() override;

        void paint(juce::Graphics& g) override;

        juce::Label label;

    private:
        Transport* transport;
    };

    //==========================================================================
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::Label currentTrackInfo;
    ElapsedTime elapsedTime;
    juce::Slider volumeSlider;
    Transport* transport;

    std::unordered_map<TransportState, std::function<void()>> stateChangeHandlers;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TransportComponent)
};
