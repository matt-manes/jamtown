#include <juce_graphics/juce_graphics.h>
#include <memory>
#include "transportComponent.h"
#include "timeFormatter.h"
#include "actionMessages.h"

TransportComponent::TransportComponent(Transport* transport)
    : transport(transport), elapsedTime(transport) {
    volumeSlider.addListener(this);
    configureInterface();
    configureHandlers();
    // transport->addChangeListener(this);
    setAudioChannels(0, 2);
    transport->setGain(volumeSlider.getValue());
    // normally called by the listener callback
    // but here transport state is already set so make manual call
    updateUI();
}

void TransportComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    transport->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void TransportComponent::getNextAudioBlock(
    const juce::AudioSourceChannelInfo& bufferToFill) {
    transport->getNextAudioBlock(bufferToFill);
}

void TransportComponent::releaseResources() { transport->releaseResources(); }

void TransportComponent::configurePlayButton() {
    playButton.onClick = [this] { playButtonClicked(); };
    playButton.setColour(juce::TextButton::buttonColourId, juce::Colours::turquoise);
    playButton.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    playButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    playButton.setButtonText("Play");
}

void TransportComponent::configureStopButton() {
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::deeppink);
    stopButton.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    stopButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
}

void TransportComponent::configureElapsedTimeLabel() {
    elapsedTime.label.setColour(juce::Label::textColourId, juce::Colours::hotpink);
}

void TransportComponent::configureVolumeSlider() {
    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    volumeSlider.setRange(0, 1, 0.000001);
    volumeSlider.setValue(0);
    volumeSlider.setSkewFactorFromMidPoint(0.1);
    volumeSlider.setTextBoxStyle(
        juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    volumeSlider.setColour(juce::Slider::ColourIds::thumbColourId,
                           juce::Colours::turquoise);
    volumeSlider.setColour(juce::Slider::ColourIds::trackColourId,
                           juce::Colours::hotpink);
}

void TransportComponent::configureInterface() {
    addAndMakeVisible(&playButton);
    configurePlayButton();

    addAndMakeVisible(&stopButton);
    configureStopButton();

    addAndMakeVisible(&currentTrackInfo);
    currentTrackInfo.setColour(juce::Label::textColourId, juce::Colours::hotpink);

    addAndMakeVisible(&elapsedTime);
    configureElapsedTimeLabel();

    addAndMakeVisible(&volumeSlider);
    configureVolumeSlider();
}

void TransportComponent::resized() {
    // TODO read these values from a file on start up
    int width = getWidth() / 4;
    stopButton.setSize(width - 10, 20);
    stopButton.setTopLeftPosition(0, getHeight() - stopButton.getHeight());
    playButton.setSize(width - 10, 20);
    playButton.setTopLeftPosition(width + 10, getHeight() - playButton.getHeight());
    volumeSlider.setSize(width, 20);
    volumeSlider.setTopLeftPosition(2 * (width + 10),
                                    getHeight() - volumeSlider.getHeight());
    auto font = currentTrackInfo.getFont();
    int displayHeight = static_cast<int>(font.getHeight() * getDisplayLineCount());
    currentTrackInfo.setSize(getWidth() / 2, displayHeight);
    currentTrackInfo.setTopLeftPosition(0, stopButton.getY() - displayHeight);
    elapsedTime.setSize(getWidth() / 2, 20);
    elapsedTime.setTopLeftPosition((getWidth() / 2) + 1, stopButton.getY() - 20);
}

void TransportComponent::paint(juce::Graphics& g) {
    g.setColour(juce::Colours::black);
    g.fillAll();
}

void TransportComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == transport) {
        updateUI();
        if (transport->trackFinished())
            sendChangeMessage();
    }
}

void TransportComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &volumeSlider) {
        transport->setGain(volumeSlider.getValue());
    }
}

void TransportComponent::updateUI() {
    if (stateChangeHandlers.contains(transport->getState()))
        stateChangeHandlers[transport->getState()]();
}

void TransportComponent::playButtonClicked() {
    if (transport->isPlaying()) {
        sendActionMessage(ActionMessages::pauseTrack);
    } else {
        sendActionMessage(ActionMessages::playTrack);
    }
}

void TransportComponent::stopButtonClicked() {
    sendActionMessage(ActionMessages::stopTrack);
}

void TransportComponent::stoppedHandler() {
    stopButton.setEnabled(false);
    playButton.setButtonText("Play");
    playButton.setEnabled(transport->hasPlayableSource() ? true : false);
}

void TransportComponent::startingHandler() { playButton.setEnabled(false); }

void TransportComponent::playingHandler() {
    stopButton.setEnabled(true);
    playButton.setButtonText("Pause");
    playButton.setEnabled(true);
    setDisplayText(transport->getCurrentTrack().toString());
}

void TransportComponent::pausedHandler() {
    playButton.setButtonText("Play");
    playButton.setEnabled(true);
    stopButton.setEnabled(true);
}

void TransportComponent::readyHandler() {
    playButton.setButtonText("Play");
    playButton.setEnabled(true);
    stopButton.setEnabled(false);
    setDisplayText(transport->getCurrentTrack().toString());
}

void TransportComponent::configureHandlers() {
    stateChangeHandlers.emplace(TransportState::STOPPED, [this] { stoppedHandler(); });
    stateChangeHandlers.emplace(TransportState::STARTING, [this] { startingHandler(); });
    stateChangeHandlers.emplace(TransportState::PLAYING, [this] { playingHandler(); });
    stateChangeHandlers.emplace(TransportState::PAUSED, [this] { pausedHandler(); });
    stateChangeHandlers.emplace(TransportState::READY, [this] { readyHandler(); });
}

int TransportComponent::getDisplayLineCount() {
    auto text = currentTrackInfo.getText();
    int count = 1;
    int pos = -1;
    while ((pos = text.indexOf(pos + 1, "\n")) != -1) {
        ++count;
    }
    return count;
}

void TransportComponent::setDisplayText(std::string text) {
    currentTrackInfo.setText(text, {});
    resized();
}

TransportComponent::ElapsedTime::ElapsedTime(Transport* transport)
    : transport(transport) {
    label.setColour(juce::Label::textColourId, juce::Colours::hotpink);
    addAndMakeVisible(label);
    setFramesPerSecond(5);
}

void TransportComponent::ElapsedTime::resized() {
    label.setBounds(0, 0, getWidth(), getHeight());
}

void TransportComponent::ElapsedTime::update() {
    if (transport->hasActiveTrack()) {
        label.setText(formatSeconds(transport->getCurrentPosition()), {});
    } else {
        label.setText("", {});
    }
}

void TransportComponent::ElapsedTime::paint(juce::Graphics& g) {
    g.setColour(juce::Colours::black);
    g.fillAll();
}
