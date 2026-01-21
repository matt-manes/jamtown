#include "searchBoxComponent.h"
#include "utilities.h"
#include <string>

SearchBoxComponent::SearchBoxComponent() {
    onTextChange = [this]() { searchUpdated(); };
    setTextToShowWhenEmpty("Search...", juce::Colours::grey);
    setColour(juce::TextEditor::backgroundColourId, juce::Colours::turquoise);
    setColour(juce::TextEditor::textColourId, juce::Colours::black);
    setColour(juce::TextEditor::focusedOutlineColourId, juce::Colours::hotpink);
    setColour(juce::TextEditor::highlightColourId, juce::Colours::cornflowerblue);
    setColour(juce::TextEditor::highlightedTextColourId, juce::Colours::hotpink);
    // the bottom of letters like 'g' tend to get clipped
    // so this is all attempting to alleviate that
    setJustification(juce::Justification::topLeft);
    setFont(juce::Font(15, 0));
    setBorder(juce::BorderSize(1));
    setIndents(4, 2);
}

void SearchBoxComponent::searchUpdated() {
    std::string text = getText().toStdString();
    utilities::toLower(text, text);
    if (text.empty() || text.substr(0, lastEntry.length()) != lastEntry) {
        // characters have been deleted or altered
        // and due to the way the algo works,
        // the search needs to be restarted
        searchService->clearSearch();
        if (!text.empty())
            searchService->search(text);
    } else {
        searchService->search(text.substr(lastEntry.length()));
    }
    lastEntry = text;
    sendActionMessage(ActionMessages::searchUpdated);
}
