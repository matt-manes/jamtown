#include "functionButtonsComponent.h"
#include "actionMessages.h"
#include <juce_gui_basics/juce_gui_basics.h>

FunctionButtonsComponent::FunctionButtonsComponent() { initializeComponent(); }

juce::Array<juce::File> FunctionButtonsComponent::getTrackAdderFiles() {
    return trackAdder.getResults();
}

void FunctionButtonsComponent::setTrackAdderWildcard(juce::String wildcard) {
    trackAdder.setScannerWildcard(wildcard);
}

void FunctionButtonsComponent::addTrackAdderListener(juce::ChangeListener* listener) {
    trackAdder.addChangeListener(listener);
}

void FunctionButtonsComponent::initializeComponent() {
    configureTrackAdder();
    configureViewLibrary();
    configureViewPlayQueue();
    // This assumes library view is the initial view
    viewLibrary.setEnabled(false);
    viewPlayQueue.setEnabled(true);
    addAndMakeVisible(trackAdder);
    addAndMakeVisible(viewLibrary);
    addAndMakeVisible(viewPlayQueue);
    configureLayout();
}

void FunctionButtonsComponent::configureTrackAdder() {
    trackAdder.addChangeListener(this);
}

void FunctionButtonsComponent::configureViewLibrary() {
    applyButtonStyle(viewLibrary);
    viewLibrary.setButtonText("Library");
    viewLibrary.onClick = [this] { viewLibraryClicked(); };
}

void FunctionButtonsComponent::configureViewPlayQueue() {
    applyButtonStyle(viewPlayQueue);
    viewPlayQueue.setButtonText("Play Queue");
    viewPlayQueue.onClick = [this] { viewPlayQueueClicked(); };
}

juce::FlexItem FunctionButtonsComponent::componentToFlexItem(
    juce::Component& component, juce::FlexItem::Margin margin) {
    return juce::FlexItem(component)
        .withFlex(0, 1, maxButtonWidth)
        .withMargin(margin)
        .withMinWidth(minButtonWidth);
}

juce::FlexItem FunctionButtonsComponent::componentToFlexItem(juce::Component& component) {
    return componentToFlexItem(component, midButtonMargin);
}

void FunctionButtonsComponent::configureLayout() {
    layoutBox.flexDirection = juce::FlexBox::Direction::row;
    layoutBox.justifyContent = juce::FlexBox::JustifyContent::flexStart;
    layoutBox.items.add(componentToFlexItem(trackAdder, leftButtonMargin));
    layoutBox.items.add(componentToFlexItem(viewLibrary));
    layoutBox.items.add(componentToFlexItem(viewPlayQueue, rightButtonMargin));
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

void FunctionButtonsComponent::resized() { layoutBox.performLayout(getLocalBounds()); }
