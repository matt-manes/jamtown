#pragma once

#include <juce_core/juce_core.h>
#include <vector>
#include <string>
#include <filesystem>
#include "trackInfo.h"

class FileScanner {
public:
    FileScanner(juce::String wildcards);

    juce::Array<juce::File> scan(juce::File dir);

private:
    juce::String wildcards;
    juce::WildcardFileFilter filter;
};
