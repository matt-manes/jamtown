#pragma once

#include <juce_core/juce_core.h>
#include <juce_audio_formats/juce_audio_formats.h>
#include <vector>
#include <memory>
#include <string>
#include "trackInfo.h"

/**
 * @brief Extracts TrackInfo objects for a list of files.
 * Files in a format that can't be read by JUCE are silently skipped.
 * Artist, Album, and Title are parsed from a file's path, not file metadata.
 * The expected path format is: /some/directory/artist/album/track.filetype
 *
 */
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

    /**
     * @brief Get the Artist name from the file path.
     *
     * @param file
     * @return std::string
     */
    std::string getArtist(juce::File file);

    /**
     * @brief Get the Album name from the file path.
     *
     * @param file
     * @return std::string
     */
    std::string getAlbum(juce::File file);

    /**
     * @brief Get the Title from the file path.
     *
     * @param file
     * @return std::string
     */
    std::string getTitle(juce::File file);
};

// TODO try multithreading?
