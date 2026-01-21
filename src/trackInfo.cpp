#include "trackInfo.h"
#include <juce_core/juce_core.h>
#include <string>
#include <algorithm>
#include "utilities.h"

std::string TrackInfo::toString() const {
    std::string info = artist + " - " + album + " - " + title + " - " + getLengthString();
    if (metadata.size() > 0) {
        info += "\nMETADATA:\n";
        for (juce::String k : metadata.getAllKeys()) {
            info +=
                k.toStdString() + ": " + metadata.getValue(k, "?").toStdString() + "\n";
        }
    }
    return info;
}

std::string TrackInfo::getLengthString() const {
    return utilities::formatSeconds(lengthInSeconds);
}

bool TrackInfo::operator==(const TrackInfo& track) { return path == track.path; }
bool TrackInfo::operator==(TrackInfo& track) { return path == track.path; }

void TrackInfo::normalizeArtist() { utilities::toLower(artist, normalizedNames.artist); }

void TrackInfo::normalizeAlbum() { utilities::toLower(album, normalizedNames.album); }

void TrackInfo::normalizeTitle() { utilities::toLower(title, normalizedNames.title); }

void TrackInfo::normalizeNames() {
    normalizeAlbum();
    normalizeArtist();
    normalizeTitle();
}

// ===============================================================================================
TrackInfoComparator::TrackInfoComparator(bool forwards) : direction(forwards ? 1 : -1) {}

void TrackInfoComparator::setDirection(bool forwards) { direction = forwards ? 1 : -1; }

bool TrackInfoComparator::goesBefore(TrackInfo first, TrackInfo second) {
    int result = compare(first, second);
    return result < 0 ? true : false;
}
// ===============================================================================================
int TitleComparator::compare(TrackInfo first, TrackInfo second) const {
    int result = first.getNormalizedTitle().compare(second.getNormalizedTitle());
    return direction * result;
}
// ===============================================================================================
AlbumComparator::AlbumComparator(bool forwards)
    : TrackInfoComparator(forwards), titleComparator(forwards) {}

int AlbumComparator::compare(TrackInfo first, TrackInfo second) const {
    int result = first.getNormalizedAlbum().compare(second.getNormalizedAlbum());
    if (result == 0) {
        result = titleComparator.compare(first, second);
    }
    return direction * result;
}

void AlbumComparator::setTrackComparatorDirection(bool forwards) {
    titleComparator.setDirection(forwards);
}
// ===============================================================================================
ArtistComparator::ArtistComparator(bool forwards)
    : TrackInfoComparator(forwards), albumComparator(forwards) {
    albumComparator.setTrackComparatorDirection(true);
}

int ArtistComparator::compare(TrackInfo first, TrackInfo second) const {
    int result = first.getNormalizedArtist().compare(second.getNormalizedArtist());
    if (result == 0) {
        result = albumComparator.compare(first, second);
    }
    return direction * result;
}
// ===============================================================================================
int LengthComparator::compare(TrackInfo first, TrackInfo second) const {
    return direction * (first.getLengthString().compare(second.getLengthString()));
}
// ===============================================================================================
TrackInfoComparator* TrackComparatorFactory::getTrackComparator(std::string attribute,
                                                                bool forwards) {
    if (attribute == "Title")
        return new TitleComparator(forwards);
    if (attribute == "Album")
        return new AlbumComparator(forwards);
    if (attribute == "Length")
        return new LengthComparator(forwards);
    // Default to artist in case I add a column and forget to make a sorter
    return new ArtistComparator(forwards);
}
// ===============================================================================================
void TrackSorter::sort(std::vector<TrackInfo>& tracks,
                       std::string attribute,
                       bool forwards) {
    auto comparator = TrackComparatorFactory::getTrackComparator(attribute, forwards);
    std::sort(
        tracks.begin(), tracks.end(), [comparator](TrackInfo first, TrackInfo second) {
            return comparator->goesBefore(first, second);
        });
    delete comparator;
}
