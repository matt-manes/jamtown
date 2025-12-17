#pragma once
#include <juce_core/juce_core.h>
#include <string>

/**
 * @brief Represents an audio track.
 *
 */
class TrackInfo {
 public:
    /**
     * @brief Construct a new Track Info object
     *
     */
    TrackInfo() {}

    /**
     * @brief Construct a new Track Info object
     *
     * @param artist The track artist.
     * @param album The album this track belongs to.
     * @param title The track title.
     * @param lengthInSeconds The track length.
     * @param path The filepath of this track.
     */
    TrackInfo(std::string artist,
              std::string album,
              std::string title,
              double lengthInSeconds,
              juce::File path)
        : artist(artist), album(album), title(title), lengthInSeconds(lengthInSeconds),
          path(path) {}

    /**
     * @brief Format track info into a string.
     *
     * @return std::string
     */
    std::string toString() const;

    // Temporary
    void setMetadata(juce::StringPairArray data) { metadata = data; }

    std::string getArtist() { return artist; }
    std::string getAlbum() { return album; }
    std::string getTitle() { return title; }
    juce::File getPath() { return path; }

    /**
     * @brief Track length in seconds.
     *
     * @return double
     */
    double getLength() { return lengthInSeconds; }

 private:
    std::string artist;
    std::string album;
    std::string title;
    double lengthInSeconds;
    juce::File path;
    juce::StringPairArray metadata;
};
