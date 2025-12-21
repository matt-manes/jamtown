#include "topBarComponent.h"
#include "actionMessages.h"

TopBarComponent::TopBarComponent() { configureElements(); }

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

int TopBarComponent::getElementWidth() {
    // return static_cast<int>(getWidth() / numElements) -
    //        ((numElements - 1) * buttonSpacing);
    return getWidth() / 10;
}

void TopBarComponent::resized() {
    int elementWidth = getElementWidth();
    int height = getHeight();
    trackAdder.setSize(elementWidth, height);
    trackAdder.setTopLeftPosition(0, 0);
    viewLibrary.setSize(elementWidth, height);
    viewLibrary.setTopLeftPosition(trackAdder.getRight() + buttonSpacing, 0);
    viewPlayQueue.setSize(elementWidth, height);
    viewPlayQueue.setTopLeftPosition(viewLibrary.getRight() + buttonSpacing, 0);
}
