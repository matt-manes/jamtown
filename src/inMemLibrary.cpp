#include "inMemLibrary.h"
#include <juce_core/juce_core.h>

void LibWriter::write(std::vector<TrackInfo> tracks) {
    auto dataDir = juce::File::getSpecialLocation(
        juce::File::SpecialLocationType::userApplicationDataDirectory);
    auto libPath = dataDir.getChildFile("jamtown/lib.txt");
    if (!libPath.exists()) {
        libPath.create();
    }
    std::string data = "";
    for (auto track : tracks) {
        data += LibWriter::prepareTrackInfo(track);
    }
    libPath.replaceWithText(data, false, false, nullptr);
}

std::string LibWriter::prepareTrackInfo(TrackInfo track) {
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

std::vector<TrackInfo> LibReader::read() {
    std::vector<TrackInfo> tracks;
    auto dataDir = juce::File::getSpecialLocation(
        juce::File::SpecialLocationType::userApplicationDataDirectory);
    auto libPath = dataDir.getChildFile("jamtown/lib.txt");
    if (!libPath.exists())
        return tracks;
    juce::FileInputStream inputStream(libPath);
    if (!inputStream.openedOk())
        return tracks;
    TrackInfo track;
    int lineno = 0;
    juce::StringPairArray metadata;
    while (!inputStream.isExhausted()) {
        auto line = inputStream.readNextLine();
        if (line == "STOP") {
            track.setMetadata(metadata);
            tracks.push_back(track);
            lineno = 0;
            track = TrackInfo();
            metadata = juce::StringPairArray();
        } else {
            if (lineno == 0) {
                track.setTitle(line.toStdString());
            } else if (lineno == 1) {
                track.setAlbum(line.toStdString());
            } else if (lineno == 2) {
                track.setArtist(line.toStdString());
            } else if (lineno == 3) {
                track.setLength(std::stod(line.toStdString()));
            } else if (lineno == 4) {
                track.setPath(juce::File(line));
            } else {
                auto key = line.substring(0, line.indexOf(":"));
                auto value = line.substring(line.indexOf(":") + 1);
                metadata.set(key, value);
            }
            ++lineno;
        }
    }
    return tracks;
}

void InMemLibrary::addTrack(TrackInfo track) {
    // This is broken for tracks with the same title
    // titleToTrackMap[track.getTitle()] = track;
    if (filepaths.contains(track.getPath().getFullPathName().toStdString()))
        return;
    filepaths.insert(track.getPath().getFullPathName().toStdString());
    tracks.push_back(track);
    albumToTrackTitleMap[track.getAlbum()].insert(track.getTitle());
    artistToAlbumMap[track.getArtist()].insert(track.getAlbum());
}

std::unordered_map<std::string, std::vector<TrackInfo>> InMemLibrary::getAlbums(
    std::string artist) {
    auto albumTitles = artistToAlbumMap[artist];
    std::unordered_map<std::string, std::vector<TrackInfo>> albums;
    for (auto album : albumTitles) {
        albums[album] = getAlbum(album);
    }
    return albums;
}

std::vector<TrackInfo> InMemLibrary::getAlbum(std::string album) {
    auto titles = albumToTrackTitleMap[album];
    std::vector<TrackInfo> tracks;
    for (auto title : titles) {
        tracks.push_back(getTrack(title));
    }
    return tracks;
}

TrackInfo InMemLibrary::getTrack(std::string title) { return titleToTrackMap[title]; }

std::vector<TrackInfo> InMemLibrary::getAllTracks() {
    return tracks;
    // std::vector<TrackInfo> tracks;
    // for (auto i = titleToTrackMap.cbegin(); i != titleToTrackMap.cend(); ++i) {
    //     tracks.push_back(i->second);
    // }
    // return tracks;
}
