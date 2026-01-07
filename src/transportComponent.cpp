#include "transportComponent.h"
#include <juce_graphics/juce_graphics.h>
#include <memory>
#include "timeFormatter.h"
#include "actionMessages.h"
#include <string>

ShuffleButton::ShuffleButton() {
    shuffleOffState.setNextState(&shuffleTrackState);
    shuffleTrackState.setNextState(&shuffleAlbumState);
    shuffleAlbumState.setNextState(&shuffleOffState);
    currentShuffleState = &shuffleOffState;
    currentShuffleState->applyState();
}

void ShuffleButtonState::applyState() { button->setButtonText(getText()); }

ShuffleButtonState* ShuffleButtonState::applyAndGetNextState() {
    next->applyState();
    return next;
}

TransportComponent::TransportComponent(Transport* transport)
    : transport(transport), elapsedTime(transport),
      skipButton("ff", 0.0, juce::Colours::turquoise),
      backButton("rw", 0.5, juce::Colours::hotpink) {
    configureInterface();
    configureHandlers();
    setAudioChannels(0, 2);
    transport->setGain(static_cast<float>(volumeSlider.getValue()));
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
    playButton.setName("play");
}

void TransportComponent::configureStopButton() {
    stopButton.setButtonText("Stop");
    stopButton.onClick = [this] { stopButtonClicked(); };
    stopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::deeppink);
    stopButton.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    stopButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    stopButton.setName("stop");
}

void TransportComponent::configureSkipButton() {
    skipButton.onClick = [this] { skipButtonClicked(); };
}

void TransportComponent::configureBackButton() {
    backButton.onClick = [this] { backButtonClicked(); };
}

void TransportComponent::configureElapsedTimeLabel() {
    elapsedTime.label.setColour(juce::Label::textColourId, juce::Colours::hotpink);
}

void TransportComponent::configureVolumeSlider() {
    volumeSlider.addListener(this);
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

void TransportComponent::configureShuffleButton() {
    shuffleButton.onClick = [this] { shuffleButtonClicked(); };
    shuffleButton.setColour(juce::TextButton::buttonColourId, juce::Colours::turquoise);
    shuffleButton.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    shuffleButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    shuffleButton.setName("shuffle");
}

void TransportComponent::configureRandomAlbumButton() {
    randomAlbumButton.onClick = [this] { randomAlbumButtonClicked(); };
    randomAlbumButton.setColour(juce::TextButton::buttonColourId,
                                juce::Colours::turquoise);
    randomAlbumButton.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    randomAlbumButton.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
    randomAlbumButton.setButtonText("Random album");
    randomAlbumButton.setName("random album");
}

void TransportComponent::configureInterface() {
    orderButtons();
    addAndMakeVisible(&playButton);
    configurePlayButton();

    addAndMakeVisible(&stopButton);
    configureStopButton();

    addAndMakeVisible(&skipButton);
    configureSkipButton();

    addAndMakeVisible(&backButton);
    configureBackButton();

    addAndMakeVisible(&currentTrackInfo);
    currentTrackInfo.setColour(juce::Label::textColourId, juce::Colours::hotpink);

    addAndMakeVisible(&elapsedTime);
    configureElapsedTimeLabel();

    addAndMakeVisible(&volumeSlider);
    configureVolumeSlider();

    addAndMakeVisible(&shuffleButton);
    configureShuffleButton();

    addAndMakeVisible(&randomAlbumButton);
    configureRandomAlbumButton();
}

void TransportComponent::resizeButtons() {
    for (auto i = buttons.begin(); i != buttons.end(); ++i) {
        juce::String name = (*i)->getName();
        int width = name == "play" || name == "stop" || name == "shuffle" ||
                            name == "random album"
                        ? 40
                        : buttonSize.width;
        (*i)->setSize(width, buttonSize.height);
        int topLeft = *i == buttons.front() ? 0 : (*(i - 1))->getRight() + 10;
        (*i)->setTopLeftPosition(topLeft, getHeight() - (*i)->getHeight());
    }
}

void TransportComponent::resized() {
    // TODO read these values from a file on start up
    resizeButtons();
    volumeSlider.setSize(static_cast<int>(getWidth() * 0.25), 20);
    volumeSlider.setTopLeftPosition(buttons.back()->getRight() + 10,
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
        transport->setGain(static_cast<float>(volumeSlider.getValue()));
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

void TransportComponent::skipButtonClicked() {
    sendActionMessage(ActionMessages::nextTrack);
}

void TransportComponent::backButtonClicked() {
    // TODO send a previous track message if current track playtime is less than 1s
    sendActionMessage(ActionMessages::restartTrack);
}

void TransportComponent::shuffleButtonClicked() {
    shuffleButton.nextState();
    sendActionMessage(ActionMessages::shuffleModeChanged);
}

void TransportComponent::randomAlbumButtonClicked() {
    sendActionMessage(ActionMessages::playRandomAlbum);
}

void TransportComponent::stoppedHandler() {
    stopButton.setEnabled(false);
    playButton.setButtonText("Play");
    bool shouldBeEnabled = transport->hasPlayableSource();
    playButton.setEnabled(shouldBeEnabled);
    skipButton.setEnabled(shouldBeEnabled);
    backButton.setEnabled(shouldBeEnabled);
}

void TransportComponent::startingHandler() { playButton.setEnabled(false); }

void TransportComponent::playingHandler() {
    stopButton.setEnabled(true);
    playButton.setButtonText("Pause");
    playButton.setEnabled(true);
    skipButton.setEnabled(true);
    backButton.setEnabled(true);
    setDisplayText(transport->getCurrentTrack().toString());
}

void TransportComponent::pausedHandler() {
    playButton.setButtonText("Play");
    playButton.setEnabled(true);
    stopButton.setEnabled(true);
    skipButton.setEnabled(true);
    backButton.setEnabled(true);
}

void TransportComponent::readyHandler() {
    playButton.setButtonText("Play");
    playButton.setEnabled(true);
    stopButton.setEnabled(false);
    skipButton.setEnabled(true);
    backButton.setEnabled(false);
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

void TransportComponent::orderButtons() {
    buttons.clear();
    buttons.push_back(&backButton);
    buttons.push_back(&stopButton);
    buttons.push_back(&playButton);
    buttons.push_back(&skipButton);
    buttons.push_back(&shuffleButton);
    buttons.push_back(&randomAlbumButton);
}

ShuffleMode TransportComponent::getCurrentShuffleMode() {
    return shuffleButton.getCurrentMode();
}
