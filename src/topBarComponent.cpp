#include "topBarComponent.h"
#include "actionMessages.h"
#include <juce_gui_basics/juce_gui_basics.h>

TopBarComponent::TopBarComponent() { configureElements(); }

juce::Array<juce::File> TopBarComponent::getTrackAdderFiles() {
    return trackAdder.getResults();
}

void TopBarComponent::setTrackAdderWildcard(juce::String wildcard) {
    trackAdder.setScannerWildcard(wildcard);
}

void TopBarComponent::configureElements() {
    configureTrackAdder();
    configureViewLibrary();
    configureViewPlayQueue();
    // This assumes library view is the initial view
    viewLibrary.setEnabled(false);
    viewPlayQueue.setEnabled(true);
}

void TopBarComponent::configureTrackAdder() {
    addAndMakeVisible(trackAdder);
    trackAdder.addChangeListener(this);
}

void TopBarComponent::configureViewLibrary() {
    addAndMakeVisible(viewLibrary);
    applyButtonStyle(viewLibrary);
    viewLibrary.setButtonText("Library");
    viewLibrary.onClick = [this] { viewLibraryClicked(); };
}

void TopBarComponent::configureViewPlayQueue() {
    addAndMakeVisible(viewPlayQueue);
    applyButtonStyle(viewPlayQueue);
    viewPlayQueue.setButtonText("Play Queue");
    viewPlayQueue.onClick = [this] { viewPlayQueueClicked(); };
}

void TopBarComponent::applyButtonStyle(juce::TextButton& button) {
    button.setColour(juce::TextButton::buttonColourId, juce::Colours::turquoise);
    button.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    button.setColour(juce::TextButton::textColourOffId, juce::Colours::black);
}

void TopBarComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &trackAdder) {
        sendActionMessage(ActionMessages::filesForLibrary);
    }
}

void TopBarComponent::viewLibraryClicked() {
    viewLibrary.setEnabled(false);
    viewPlayQueue.setEnabled(true);
    sendActionMessage(ActionMessages::viewLibrary);
}

void TopBarComponent::viewPlayQueueClicked() {
    viewLibrary.setEnabled(true);
    viewPlayQueue.setEnabled(false);
    sendActionMessage(ActionMessages::viewPlayQueue);
}

void TopBarComponent::resized() {
    juce::FlexBox box;
    box.flexDirection = juce::FlexBox::Direction::row;
    box.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    box.items.add(juce::FlexItem(trackAdder).withFlex(1).withMargin(leftButtonMargin));
    box.items.add(juce::FlexItem(viewLibrary).withFlex(1).withMargin(midButtonMargin));
    box.items.add(
        juce::FlexItem(viewPlayQueue).withFlex(1).withMargin(rightButtonMargin));
    box.performLayout(getLocalBounds());
}
