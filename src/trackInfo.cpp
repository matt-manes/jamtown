#include "trackInfo.h"
#include <juce_core/juce_core.h>
#include <string>

std::string TrackInfo::toString() const {
    int minutes = static_cast<int>(static_cast<int>(lengthInSeconds / 60) % 60);
    int seconds = static_cast<int>(static_cast<int>(lengthInSeconds) % 60);
    std::string info = artist + " - " + album + " - " + title + " - ";
    if (seconds == 0)
        info += std::format("{0}:00", minutes);
    else if (seconds < 10)
        info += std::format("{0}:0{1}", minutes, seconds);
    else
        info += std::format("{0}:{1}", minutes, seconds);
    if (metadata.size() > 0) {
        info += "\nMETADATA:\n";
        for (juce::String k : metadata.getAllKeys()) {
            info +=
                k.toStdString() + ": " + metadata.getValue(k, "?").toStdString() + "\n";
        }
    }
    return info;
}
