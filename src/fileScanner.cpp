#include "fileScanner.h"

FileScanner::FileScanner(juce::String wildcards)
    : filter(wildcards, "", ""), wildcards(wildcards) {}

juce::Array<juce::File> FileScanner::scan(juce::File dir) {
    juce::FileSearchPath searchPath(dir.getFullPathName());
    return searchPath.findChildFiles(juce::File::findFiles, true, wildcards);
}
