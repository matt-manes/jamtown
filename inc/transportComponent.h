#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_events/juce_events.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <vector>
#include <string>
#include <unordered_map>
#include "transportController.h"
#include "scrubSlider.h"

enum ShuffleMode { OFF, TRACK, ALBUM };

/**
 * @brief Class for adding different shuffle settings to the transports shuffle button.
 *
 */
class ShuffleButtonState {
public:
    /**
     * @brief Construct a new Shuffle Button State object
     *
     * @param button A pointer to the shuffle button.
     * @param mode The shuffle type this state represents.
     * @param buttonText The text this state should display when active.
     */
    ShuffleButtonState(juce::TextButton* button, ShuffleMode mode, std::string buttonText)
        : button(button), mode(mode), text(buttonText) {}

    /**
     * @brief Get the shuffle type of this state.
     *
     * @return ShuffleMode
     */
    ShuffleMode getMode() { return mode; }

    /**
     * @brief Get the text this state displays when active.
     *
     * @return std::string
     */
    std::string getText() { return text; }

    /**
     * @brief Calls the next state's `applyState` method and then returns the state.
     *
     * @return ShuffleButtonState*
     */
    ShuffleButtonState* transistionToNextState();

    /**
     * @brief Called when transistioning to this state.
     *
     */
    virtual void applyState();

    /**
     * @brief Set what state should be transistioned to when this state's `transistionToNextState` method is called.
     *
     * @param state
     */
    void setNextState(ShuffleButtonState* state) { this->nextState = state; }

private:
    ShuffleButtonState* nextState;
    ShuffleMode mode;
    std::string text;
    juce::TextButton* button;
};

/**
 * @brief The shuffle button with circular mode cycling.
 *
 */
class ShuffleButton : public juce::TextButton {
public:
    ShuffleButton();

    /**
     * @brief Change to the next shuffle mode.
     *
     */
    void transistionToNextState() {
        currentShuffleState = currentShuffleState->transistionToNextState();
    }

    /**
     * @brief Get the current shuffle mode.
     *
     * @return ShuffleMode
     */
    ShuffleMode getCurrentMode() { return currentShuffleState->getMode(); }

private:
    ShuffleButtonState shuffleOffState{this, ShuffleMode::OFF, "Shuffle off"};
    ShuffleButtonState shuffleTrackState{this, ShuffleMode::TRACK, "Shuffle tracks"};
    ShuffleButtonState shuffleAlbumState{this, ShuffleMode::ALBUM, "Shuffle albums"};
    ShuffleButtonState* currentShuffleState;

    /**
     * @brief Set up should be done here and called from the constructor.
     *
     */
    void initializeComponent();
};

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
    TransportComponent(TransportController* transportController);

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

    ShuffleMode getCurrentShuffleMode();

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

    void skipButtonClicked();

    void backButtonClicked();

    void shuffleButtonClicked();

    void randomAlbumButtonClicked();

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
    void configureVolumeSlider();
    void configureVolumeLabel();
    void configureScrubSlider();
    void configureSkipButton();
    void configureBackButton();
    void configureShuffleButton();
    void configureRandomAlbumButton();
    void configureTrackInfoBox();
    void configureControlBox();
    void configureTransportBox();

    /**
     * @brief Set up should be done here and called from the constructor.
     *
     */
    void initializeComponent();

    std::string getCurrentTrackDisplayString();

    void setCurrenTrackDisplayString(std::string text);

    /**
     * @brief Sets the left to right order control buttons should appear.
     *
     */
    void orderButtons();

    //==========================================================================
    float textButtonWidth = 75.0f;
    float arrowButtonWidth = 20.0f;
    float volumeSliderWidth = 200.0f;
    std::vector<juce::Component*> buttons;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::ArrowButton skipButton;
    juce::ArrowButton backButton;
    ShuffleButton shuffleButton;
    juce::TextButton randomAlbumButton;
    juce::Label currentTrackInfo;
    juce::Label volumeLabel;
    juce::Slider volumeSlider;
    ScrubSlider scrubSlider;
    TransportController* transportController;
    juce::FlexBox trackInfoBox;
    juce::FlexBox controlsBox;
    // trackInfoBox and controlsBox are held by this
    juce::FlexBox transportBox;

    std::unordered_map<TransportState, std::function<void()>> stateChangeHandlers;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TransportComponent)
};
