#pragma once

#include <juce_core/juce_core.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <vector>
#include <memory>
#include "trackInfo.h"

class FileProcessor {
public:
    FileProcessor();

    /**
     * @brief Read track info from the given files.
     *
     * @param files
     * @return std::vector<TrackInfo>
     */
    std::vector<TrackInfo> processFiles(juce::Array<juce::File> files);

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
};

// TODO try multithreading?
