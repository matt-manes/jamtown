#include "trackInfo.h"
#include <juce_core/juce_core.h>
#include <string>

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
    int minutes = static_cast<int>(static_cast<int>(lengthInSeconds / 60) % 60);
    int seconds = static_cast<int>(static_cast<int>(lengthInSeconds) % 60);
    if (seconds == 0)
        return std::format("{0}:00", minutes);
    else if (seconds < 10)
        return std::format("{0}:0{1}", minutes, seconds);
    else
        return std::format("{0}:{1}", minutes, seconds);
}

bool TrackInfo::operator==(const TrackInfo& track) { return path == track.path; }
