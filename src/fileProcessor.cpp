#include "fileProcessor.h"
#include <vector>
#include <string>

FileProcessor::FileProcessor() { formatManager.registerBasicFormats(); }

std::vector<TrackInfo> FileProcessor::processFiles(juce::Array<juce::File> files) {
    std::vector<TrackInfo> tracks;
    for (auto file : files) {
        auto* reader = formatManager.createReaderFor(file);
        if (reader == nullptr)
            continue;
        double lengthInSeconds =
            static_cast<double>(reader->lengthInSamples) / reader->sampleRate;
        std::string artist =
            file.getParentDirectory().getParentDirectory().getFileName().toStdString();
        std::string album = file.getParentDirectory().getFileName().toStdString();
        std::string title = file.getFileNameWithoutExtension().toStdString();
        tracks.emplace_back(TrackInfo(artist, album, title, lengthInSeconds, file));
        tracks[tracks.size() - 1].setMetadata(reader->metadataValues);
        delete reader;
    }
    return tracks;
}
