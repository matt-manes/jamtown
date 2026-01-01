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
    if (filepaths.contains(track.getPath().getFullPathName().toStdString()))
        return;
    filepaths.insert(track.getPath().getFullPathName().toStdString());
    tracks.push_back(track);
    db[track.getArtist()][track.getAlbum()].push_back(track);
}

std::unordered_map<std::string, std::vector<TrackInfo>> InMemLibrary::getAlbums(
    std::string artist) {
    if (db.contains(artist))
        return db[artist];
    return std::unordered_map<std::string, std::vector<TrackInfo>>{};
}

std::vector<TrackInfo> InMemLibrary::getAlbumTracks(std::string album,
                                                    std::string artist) {
    if (db.contains(artist) && db[artist].contains(album))
        return db[artist][album];
    return std::vector<TrackInfo>{};
}

TrackInfo InMemLibrary::getTrack(std::string title,
                                 std::string album,
                                 std::string artist) {
    // Not ideal way to do look up time-complexity wise
    // but the number of tracks in an album is generally small
    auto tracks = getAlbumTracks(album, artist);
    for (auto track : tracks) {
        if (track.getTitle() == title) {
            return track;
        }
    }
    return TrackInfo{};
}

std::vector<TrackInfo> InMemLibrary::getAllTracks() { return tracks; }

void InMemLibrary::removeTrack(std::string title, std::string album, std::string artist) {
    TrackInfo track = getTrack(title, album, artist);
    if (track.getTitle() == title) {
        filepaths.erase(track.getPath().getFullPathName().toStdString());
        std::erase_if(db[artist][album], [track](TrackInfo t) { return t == track; });
        std::erase_if(tracks, [track](TrackInfo t) { return t == track; });
        if (db[artist][album].empty())
            db[artist].erase(album);
        if (db[artist].empty())
            db.erase(artist);
    }
}
