#include "addTracksComponent.h"

AddTracksComponent::AddTracksComponent() : fileScanner("*") {
    addAndMakeVisible(addTracksButton);
    addTracksButton.setButtonText("Add Tracks");
    addTracksButton.setColour(juce::TextButton::buttonColourId, juce::Colours::turquoise);
    addTracksButton.onClick = [this] { onClick(); };
}

void AddTracksComponent::paint(juce::Graphics& g) {
    g.setColour(juce::Colours::black);
    g.fillAll();
}

void AddTracksComponent::resized() {
    addTracksButton.setBounds(0, 0, getWidth(), getHeight());
}

juce::Array<juce::File> AddTracksComponent::getResults() {
    juce::Array<juce::File> results = gatheredFiles;
    gatheredFiles = juce::Array<juce::File>{};
    return results;
}

void AddTracksComponent::setScannerWildcard(juce::String wildcard) {
    fileScanner = FileScanner(wildcard);
    supportedFormatsWildcard = wildcard;
}

void AddTracksComponent::onClick() {
    chooser = std::make_unique<juce::FileChooser>(
        "Select folder", lastOpened, supportedFormatsWildcard);
    auto chooserFlags = juce::FileBrowserComponent::openMode |
                        juce::FileBrowserComponent::canSelectDirectories;
    // juce::FileBrowserComponent::canSelectFiles;
    // juce::FileBrowserComponent::canSelectMultipleItems;

    chooser->launchAsync(chooserFlags, [this](const juce::FileChooser& fc) {
        auto entry = fc.getResult();
        if (entry == juce::File{})
            return;
        if (entry.isDirectory()) {
            gatheredFiles.addArray(fileScanner.scan(entry));
        } else {
            gatheredFiles.add(entry);
        }
        lastOpened = entry.getParentDirectory();
        sendChangeMessage();
    });
}
