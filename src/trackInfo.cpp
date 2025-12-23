#include "trackInfo.h"
#include <juce_core/juce_core.h>
#include <string>
#include "timeFormatter.h"

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

std::string TrackInfo::getLengthString() const { return formatSeconds(lengthInSeconds); }

bool TrackInfo::operator==(const TrackInfo& track) { return path == track.path; }
