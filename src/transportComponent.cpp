#include "transportComponent.h"
#include <juce_graphics/juce_graphics.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>
#include <limits>
#include "actionMessages.h"
#include <string>
#include "utilities.h"

ShuffleButton::ShuffleButton() {
    shuffleOffState.setNextState(&shuffleTrackState);
    shuffleTrackState.setNextState(&shuffleAlbumState);
    shuffleAlbumState.setNextState(&shuffleOffState);
    currentShuffleState = &shuffleOffState;
    currentShuffleState->applyState();
}

void ShuffleButtonState::applyState() { button->setButtonText(getText()); }

ShuffleButtonState* ShuffleButtonState::transistionToNextState() {
    nextState->applyState();
    return nextState;
}

TransportComponent::TransportComponent(TransportController* transport)
    : transportController(transport), skipButton("ff", 0.0, juce::Colours::turquoise),
      backButton("rw", 0.5, juce::Colours::hotpink), scrubSlider(transport) {
    initializeComponent();
    configureHandlers();
    setAudioChannels(0, 2);
    transport->setGain(static_cast<float>(volumeSlider.getValue()));
    // normally called by the listener callback
    // but here transport state is already set so make manual call
    updateUI();
}

void TransportComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
    transportController->prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void TransportComponent::getNextAudioBlock(
    const juce::AudioSourceChannelInfo& bufferToFill) {
    transportController->getNextAudioBlock(bufferToFill);
}

void TransportComponent::releaseResources() { transportController->releaseResources(); }

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
    skipButton.setName("skip");
}

void TransportComponent::configureBackButton() {
    backButton.onClick = [this] { backButtonClicked(); };
    backButton.setName("back");
}

void TransportComponent::configureVolumeSlider() {
    volumeSlider.addListener(this);
    volumeSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    volumeSlider.setRange(0, 1, std::numeric_limits<double>::min());
    volumeSlider.setValue(0);
    volumeSlider.setSkewFactorFromMidPoint(0.1);
    volumeSlider.setTextBoxStyle(
        juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    volumeSlider.setColour(juce::Slider::ColourIds::thumbColourId,
                           juce::Colours::turquoise);
    volumeSlider.setColour(juce::Slider::ColourIds::trackColourId,
                           juce::Colours::hotpink);
}

void TransportComponent::configureVolumeLabel() {
    volumeLabel.setJustificationType(juce::Justification::centredRight);
    volumeLabel.setColour(juce::Label::textColourId, juce::Colours::hotpink);
    volumeLabel.setText("vol", {});
}

void TransportComponent::configureScrubSlider() { scrubSlider.slider.addListener(this); }

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

void TransportComponent::configureTrackInfoBox() {
    currentTrackInfo.setColour(juce::Label::textColourId, juce::Colours::hotpink);
    currentTrackInfo.setJustificationType(juce::Justification::centred);
    trackInfoBox.flexDirection = juce::FlexBox::Direction::row;
    trackInfoBox.justifyContent = juce::FlexBox::JustifyContent::center;
    trackInfoBox.items.add(juce::FlexItem(currentTrackInfo).withFlex(1));
}

void TransportComponent::configureControlBox() {
    controlsBox.flexDirection = juce::FlexBox::Direction::row;
    controlsBox.justifyContent = juce::FlexBox::JustifyContent::center;
    juce::FlexItem::Margin controlMargin{0, 2.5, 0, 2.5};
    float width;
    float minWidth;
    juce::String name;
    for (auto i = buttons.begin(); i != buttons.end(); ++i) {
        name = (*i)->getName();
        if (name == "skip" || name == "back") {
            width = arrowButtonWidth;
            minWidth = arrowButtonWidth / 2.0f;
        } else {
            width = textButtonWidth;
            minWidth = 0.0f;
        }
        controlsBox.items.add(juce::FlexItem(*(*i))
                                  .withFlex(0, 1, width)
                                  .withMargin(controlMargin)
                                  .withMinWidth(minWidth));
    }
    controlsBox.items.add(juce::FlexItem(volumeLabel).withFlex(0, 1, 30));
    controlsBox.items.add(juce::FlexItem(volumeSlider).withFlex(0, 1, volumeSliderWidth));
}

void TransportComponent::configureTransportBox() {
    juce::FlexItem::Margin margin{0, 0, 1, 0};
    transportBox.flexDirection = juce::FlexBox::Direction::column;
    transportBox.items.addArray(
        {juce::FlexItem(trackInfoBox).withFlex(1).withMargin(margin),
         juce::FlexItem(scrubSlider).withFlex(1).withMargin(margin),
         juce::FlexItem(controlsBox).withFlex(1).withMargin(margin)});
}

void TransportComponent::initializeComponent() {
    orderButtons();
    configurePlayButton();
    addAndMakeVisible(&playButton);

    configureStopButton();
    addAndMakeVisible(&stopButton);

    configureSkipButton();
    addAndMakeVisible(&skipButton);

    configureBackButton();
    addAndMakeVisible(&backButton);

    addAndMakeVisible(&currentTrackInfo);

    configureVolumeSlider();
    addAndMakeVisible(&volumeSlider);
    configureVolumeLabel();
    addAndMakeVisible(&volumeLabel);

    configureShuffleButton();
    addAndMakeVisible(&shuffleButton);

    configureRandomAlbumButton();
    addAndMakeVisible(&randomAlbumButton);

    configureScrubSlider();
    addAndMakeVisible(&scrubSlider);

    configureTrackInfoBox();
    configureControlBox();
    configureTransportBox();
}

void TransportComponent::resized() { transportBox.performLayout(getLocalBounds()); }

void TransportComponent::paint(juce::Graphics& g) {
    g.setColour(juce::Colours::black);
    g.fillAll();
}

void TransportComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == transportController) {
        updateUI();
        if (transportController->trackFinished())
            sendChangeMessage();
    }
}

void TransportComponent::sliderValueChanged(juce::Slider* slider) {
    if (slider == &volumeSlider) {
        transportController->setGain(static_cast<float>(volumeSlider.getValue()));
    } else if (slider == &scrubSlider.slider) {
        transportController->setPosition(scrubSlider.slider.getValue());
    }
}

void TransportComponent::updateUI() {
    if (stateChangeHandlers.contains(transportController->getState()))
        stateChangeHandlers[transportController->getState()]();
}

void TransportComponent::playButtonClicked() {
    if (transportController->isPlaying()) {
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
    shuffleButton.transistionToNextState();
    sendActionMessage(ActionMessages::shuffleModeChanged);
}

void TransportComponent::randomAlbumButtonClicked() {
    sendActionMessage(ActionMessages::playRandomAlbum);
}

void TransportComponent::stoppedHandler() {
    stopButton.setEnabled(false);
    playButton.setButtonText("Play");
    bool shouldBeEnabled = transportController->hasPlayableSource();
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
    setDisplayText(getCurrentTrackDisplayString());
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
    setDisplayText(getCurrentTrackDisplayString());
}

void TransportComponent::configureHandlers() {
    stateChangeHandlers.emplace(TransportState::STOPPED, [this] { stoppedHandler(); });
    stateChangeHandlers.emplace(TransportState::STARTING, [this] { startingHandler(); });
    stateChangeHandlers.emplace(TransportState::PLAYING, [this] { playingHandler(); });
    stateChangeHandlers.emplace(TransportState::PAUSED, [this] { pausedHandler(); });
    stateChangeHandlers.emplace(TransportState::READY, [this] { readyHandler(); });
}

std::string TransportComponent::getCurrentTrackDisplayString() {
    TrackInfo track = transportController->getCurrentTrack();
    return track.getArtist() + " - " + track.getAlbum() + " - " + track.getTitle() +
           " - " + track.getLengthString();
}

void TransportComponent::setDisplayText(std::string text) {
    currentTrackInfo.setText(text, {});
    resized();
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
