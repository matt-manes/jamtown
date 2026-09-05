#include "functionButtonsComponent.h"
#include "actionMessages.h"
#include <juce_gui_basics/juce_gui_basics.h>

FunctionButtonsComponent::FunctionButtonsComponent() { configureElements(); }

juce::Array<juce::File> FunctionButtonsComponent::getTrackAdderFiles() {
    return trackAdder.getResults();
}

void FunctionButtonsComponent::setTrackAdderWildcard(juce::String wildcard) {
    trackAdder.setScannerWildcard(wildcard);
}

void FunctionButtonsComponent::configureElements() {
    configureTrackAdder();
    configureViewLibrary();
    configureViewPlayQueue();
    // This assumes library view is the initial view
    viewLibrary.setEnabled(false);
    viewPlayQueue.setEnabled(true);
}

void FunctionButtonsComponent::configureTrackAdder() {
    addAndMakeVisible(trackAdder);
    trackAdder.addChangeListener(this);
}

void FunctionButtonsComponent::configureViewLibrary() {
    addAndMakeVisible(viewLibrary);
    applyButtonStyle(viewLibrary);
    viewLibrary.setButtonText("Library");
    viewLibrary.onClick = [this] { viewLibraryClicked(); };
}

void FunctionButtonsComponent::configureViewPlayQueue() {
    addAndMakeVisible(viewPlayQueue);
    applyButtonStyle(viewPlayQueue);
    viewPlayQueue.setButtonText("Play Queue");
    viewPlayQueue.onClick = [this] { viewPlayQueueClicked(); };
}

void FunctionButtonsComponent::applyButtonStyle(juce::TextButton& button) {
    button.setColour(juce::TextButton::buttonColourId, juce::Colours::turquoise);
    button.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    button.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
}

void FunctionButtonsComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &trackAdder) {
        sendActionMessage(ActionMessages::filesForLibrary);
    }
}

void FunctionButtonsComponent::viewLibraryClicked() {
    viewLibrary.setEnabled(false);
    viewPlayQueue.setEnabled(true);
    sendActionMessage(ActionMessages::viewLibrary);
}

void FunctionButtonsComponent::viewPlayQueueClicked() {
    viewLibrary.setEnabled(true);
    viewPlayQueue.setEnabled(false);
    sendActionMessage(ActionMessages::viewPlayQueue);
}

void FunctionButtonsComponent::resized() {
    juce::FlexBox box;
    box.flexDirection = juce::FlexBox::Direction::row;
    box.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    box.items.add(juce::FlexItem(trackAdder)
                      .withFlex(1)
                      .withMargin(leftButtonMargin)
                      .withMaxWidth(maxButtonWidth));
    box.items.add(juce::FlexItem(viewLibrary)
                      .withFlex(1)
                      .withMargin(midButtonMargin)
                      .withMaxWidth(maxButtonWidth));
    box.items.add(juce::FlexItem(viewPlayQueue)
                      .withFlex(1)
                      .withMargin(rightButtonMargin)
                      .withMaxWidth(maxButtonWidth));
    box.performLayout(getLocalBounds());
}
