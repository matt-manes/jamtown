#pragma once

#include <juce_core/juce_core.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <vector>
#include <memory>
#include <string>
#include "trackInfo.h"

class FileProcessor {
public:
    FileProcessor();

    std::vector<TrackInfo> processFiles(juce::Array<juce::File> files);

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
};

// TODO try multithreading?
