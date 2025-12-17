#include "fileProcessor.h"

FileProcessor::FileProcessor() { formatManager.registerBasicFormats(); }

std::vector<TrackInfo> FileProcessor::processFiles(juce::Array<juce::File> files) {
    std::vector<TrackInfo> tracks;
    for (auto file : files) {
        auto* reader = formatManager.createReaderFor(file);
        if (reader == nullptr)
            continue;
        double lengthInSeconds =
            static_cast<double>(reader->lengthInSamples) / reader->sampleRate;
        delete reader;
        std::string artist =
            file.getParentDirectory().getParentDirectory().getFileName().toStdString();
        std::string album = file.getParentDirectory().getFileName().toStdString();
        std::string title = file.getFileNameWithoutExtension().toStdString();
        tracks.emplace_back(TrackInfo(artist, album, title, lengthInSeconds, file));
    }
    return tracks;
}
