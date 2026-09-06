#include "topBarComponent.h"
#include "searchService.h"
#include "searchBoxComponent.h"
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

void TopBarComponent::resized() { layoutBox.performLayout(getLocalBounds()); }

void TopBarComponent::configureLayout() {
    layoutBox.flexDirection = juce::FlexBox::Direction::row;
    layoutBox.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    layoutBox.items.add(juce::FlexItem(functionButtons).withFlex(1));
    // empty box to keep spacing gap
    layoutBox.items.add(juce::FlexItem().withFlex(1));
    layoutBox.items.add(juce::FlexItem(searchBox).withFlex(1));
}

void TopBarComponent::configureElements() {
    addAndMakeVisible(functionButtons);
    addAndMakeVisible(searchBox);
    configureLayout();
}

void TopBarComponent::addFunctionButtonsActionListener(juce::ActionListener* listener) {
    functionButtons.addActionListener(listener);
}

void TopBarComponent::setTrackAdderWildcard(juce::String wildcard) {
    functionButtons.setTrackAdderWildcard(wildcard);
}

void TopBarComponent::addSearchBoxActionListener(juce::ActionListener* listener) {
    searchBox.addActionListener(listener);
}

juce::Array<juce::File> TopBarComponent::getTrackAdderFiles() {
    return functionButtons.getTrackAdderFiles();
}
