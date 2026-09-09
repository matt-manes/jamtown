#include "scrubSlider.h"
#include "utilities.h"

ScrubSlider::ScrubSlider(TransportController* transportController)
    : transportController(transportController) {
    initializeComponent();
}

void ScrubSlider::configureSlider() {
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
    slider.setRange(0, 1, 1);
    slider.setValue(0);
    slider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
    slider.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::turquoise);
    slider.setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::hotpink);
}

void ScrubSlider::configureLabels() {
    elapsedLabel.setColour(juce::Label::textColourId, juce::Colours::hotpink);
    elapsedLabel.setJustificationType(juce::Justification::centred);
    remainingLabel.setColour(juce::Label::textColourId, juce::Colours::hotpink);
    remainingLabel.setJustificationType(juce::Justification::centred);
}

void ScrubSlider::configureLayout() {
    layoutBox.flexDirection = juce::FlexBox::Direction::row;
    layoutBox.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    juce::FlexItem::Margin margin{};
    layoutBox.items.addArray(
        {juce::FlexItem(elapsedLabel).withFlex(0, 1, labelWidth).withMargin(margin),
         juce::FlexItem(slider).withFlex(1),
         juce::FlexItem(remainingLabel).withFlex(0, 1, labelWidth).withMargin(margin)});
}

void ScrubSlider::initializeComponent() {
    setFramesPerSecond(60);
    transportController->addChangeListener(this);
    configureSlider();
    addAndMakeVisible(slider);
    configureLabels();
    addAndMakeVisible(elapsedLabel);
    addAndMakeVisible(remainingLabel);
    configureLayout();
}

void ScrubSlider::resized() { layoutBox.performLayout(getLocalBounds()); }

void ScrubSlider::update() {
    if (transportController->hasActiveTrack()) {
        slider.setValue(transportController->getCurrentPosition(),
                        juce::NotificationType::dontSendNotification);
        double elapsedSeconds = transportController->getCurrentPosition();
        TrackInfo track = transportController->getCurrentTrack();
        elapsedLabel.setText(utilities::formatSeconds(elapsedSeconds), {});
        remainingLabel.setText(
            utilities::formatSeconds(track.getLength() - elapsedSeconds), {});
    } else {
        slider.setValue(0, juce::NotificationType::dontSendNotification);
        elapsedLabel.setText("0:00", {});
        remainingLabel.setText("", {});
    }
}

void ScrubSlider::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == transportController) {
        TrackInfo track = transportController->getCurrentTrack();
        slider.setRange(0, track.getLength(), .001);
    }
}
