#include "topBarComponent.h"
#include "searchService.h"
#include "searchBoxComponent.h"
#include <juce_core/juce_core.h>
#include <juce_gui_basics/juce_gui_basics.h>

void TopBarComponent::resized() {
    juce::FlexBox box;
    box.flexDirection = juce::FlexBox::Direction::row;
    box.justifyContent = juce::FlexBox::JustifyContent::spaceBetween;
    box.items.add(juce::FlexItem(functionButtons).withFlex(1));
    box.items.add(juce::FlexItem(searchBox).withFlex(1).withMaxWidth(getWidth() * 0.33f));
    box.performLayout(getLocalBounds());
}

void TopBarComponent::init() {
    addAndMakeVisible(functionButtons);
    addAndMakeVisible(searchBox);
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
