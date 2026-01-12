#pragma once

#include <juce_core/juce_core.h>
#include <vector>
#include <string>
#include <filesystem>
#include "trackInfo.h"

class FileScanner {
public:
    /**
     * @brief Construct a new File Scanner object
     *
     * @param wildcards The wildcard pattern to use when scanning for files, ';' seperated. e.g. "*.wav;*.mp3".
     */
    FileScanner(juce::String wildcards);

    /**
     * @brief Scan the given directory recursively for files that match this instance's wildcard pattern.
     *
     * @param dir
     * @return juce::Array<juce::File>
     */
    juce::Array<juce::File> scan(juce::File dir);

private:
    juce::String wildcards;
    juce::WildcardFileFilter filter;
};
