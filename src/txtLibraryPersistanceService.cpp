#include "txtLibraryPersistanceService.h"
#include <juce_core/juce_core.h>
#include <vector>
#include <string>

TxtLibraryPersistanceService::TxtLibraryPersistanceService() {
#ifdef USE_DEV_LIB
    // debug mode is slow to load large library files
    relativeLibPath = "jamtown/libdev.txt";
#else
    relativeLibPath = "jamtown/lib.txt";
#endif
}

juce::File TxtLibraryPersistanceService::getLibFile() {
    return juce::File::getSpecialLocation(
               juce::File::SpecialLocationType::userApplicationDataDirectory)
        .getChildFile(relativeLibPath);
}

std::string TxtLibraryPersistanceService::prepareTrackInfoForSave(TrackInfo track) {
    std::string info = "";
    info += track.getTitle() + "\n";
    info += track.getAlbum() + "\n";
    info += track.getArtist() + "\n";
    info += std::to_string(track.getLength()) + "\n";
    info += track.getPath().getFullPathName().toStdString() + "\n";
    auto metadata = track.getMetadata();
    for (juce::String k : metadata.getAllKeys()) {
        info += k.toStdString() + ":" + metadata.getValue(k, "?").toStdString() + "\n";
    }
    info += "STOP\n";
    return info;
}

void TxtLibraryPersistanceService::save(std::vector<TrackInfo> tracks) {
    auto libPath = getLibFile();
    if (!libPath.exists()) {
        libPath.create();
    }
    std::string data = "";
    for (auto track : tracks) {
        data += prepareTrackInfoForSave(track);
    }
    if (!libPath.replaceWithText(data, false, false, nullptr))
        throw LibFileException("Failed to save library file.");
}

std::vector<TrackInfo> TxtLibraryPersistanceService::load() {
    std::vector<TrackInfo> tracks;
    auto libPath = getLibFile();
    if (!libPath.exists())
        return tracks;

    juce::FileInputStream inputStream(libPath);
    if (!inputStream.openedOk())
        throw LibFileException("Unable to open lib file.");

    TrackInfo track;
    int trackLineNo = 0;
    int fileLineNo = 0;
    juce::StringPairArray metadata;

    while (!inputStream.isExhausted()) {
        auto line = inputStream.readNextLine();
        if (line == "STOP") {
            track.setMetadata(metadata);
            tracks.push_back(track);
            trackLineNo = 0;
            track = TrackInfo();
            metadata.clear();
        } else {
            switch (trackLineNo) {
            case 0:
                track.setTitle(line.toStdString());
                break;
            case 1:
                track.setAlbum(line.toStdString());
                break;
            case 2:
                track.setArtist(line.toStdString());
                break;
            case 3:
                try {
                    track.setLength(std::stod(line.toStdString()));
                } catch (const std::invalid_argument&) {
                    throw LibFileException("Unable to convert '" + line.toStdString() +
                                           "' to double at line " +
                                           std::to_string(fileLineNo) + ".");
                }
                break;
            case 4:
                track.setPath(juce::File(line));
                break;
            default:
                // parse metadata key-value pair
                auto colonIndex = line.indexOf(":");
                if (colonIndex == -1)
                    throw LibFileException("Missing colon separator in metadata row '" +
                                           line.toStdString() + "' at line " +
                                           std::to_string(fileLineNo) + ".");
                auto key = line.substring(0, colonIndex);
                auto value = line.substring(colonIndex + 1);
                metadata.set(key, value);
                break;
            }
            ++trackLineNo;
        }
        ++fileLineNo;
    }
    return tracks;
}
