#pragma once
#include <juce_core/juce_core.h>
#include <string>

class TrackInfo {
 public:
    TrackInfo() {}

    TrackInfo(std::string artist,
              std::string album,
              std::string title,
              double lengthInSeconds,
              juce::File path)
        : artist(artist), album(album), title(title), lengthInSeconds(lengthInSeconds),
          path(path) {}

    std::string toString() const;

    void setMetadata(juce::StringPairArray data) { metadata = data; }

 private:
    std::string artist;
    std::string album;
    std::string title;
    double lengthInSeconds;
    juce::File path;
    juce::StringPairArray metadata;
};
