#include "fileProcessor.h"
#include <vector>
#include <string>

FileProcessor::FileProcessor() { formatManager.registerBasicFormats(); }

std::vector<TrackInfo> FileProcessor::processFiles(juce::Array<juce::File> files) {
    std::vector<TrackInfo> tracks;
    for (auto file : files) {
        auto* reader = formatManager.createReaderFor(file);
        if (reader == nullptr)
            // skip files that can't be read
            continue;
        double lengthInSeconds =
            static_cast<double>(reader->lengthInSamples) / reader->sampleRate;
        tracks.emplace_back(TrackInfo(
            getArtist(file), getAlbum(file), getTitle(file), lengthInSeconds, file));
        tracks[tracks.size() - 1].setMetadata(reader->metadataValues);
        delete reader;
    }
    return tracks;
}

std::string FileProcessor::getArtist(juce::File file) {
    // getParentDirectory returns the root if you're at the root, so no penalty for not checking first
    return file.getParentDirectory().getParentDirectory().getFileName().toStdString();
}

std::string FileProcessor::getAlbum(juce::File file) {
    // getParentDirectory returns the root if you're at the root, so no penalty for not checking first
    return file.getParentDirectory().getFileName().toStdString();
}

std::string FileProcessor::getTitle(juce::File file) {
    return file.getFileNameWithoutExtension().toStdString();
}
