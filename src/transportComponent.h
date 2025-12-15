#pragma once

#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_events/juce_events.h>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include "transport.h"

class TransportComponent : public juce::AudioAppComponent, public juce::ChangeListener {
 public:
    TransportComponent();

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

    void setGain(float gain) { transport.setGain(gain); }

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    std::string getStateString() { return stateMap[transport.getState()]; }

 private:
    void changeState(TransportState state) { transport.changeState(state); }

    void updateUIState();

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

    //==========================================================================
    juce::TextButton openButton;
    juce::TextButton playButton;
    juce::TextButton stopButton;
    juce::Label display;
    juce::Label stateLabel;

    std::unique_ptr<juce::FileChooser> chooser;

    Transport transport;

    std::unordered_map<TransportState, std::function<void()>> handlers;
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
