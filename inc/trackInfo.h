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

    std::string getLengthString() const;

    bool operator==(const TrackInfo& track);

private:
    std::string artist;
    std::string album;
    std::string title;
    double lengthInSeconds;
    juce::File path;
    juce::StringPairArray metadata;
};

class TrackInfoComparator {
public:
    TrackInfoComparator(bool forwards);
    virtual ~TrackInfoComparator() = default;

    virtual int compare(TrackInfo first, TrackInfo second) const = 0;
    bool goesBefore(TrackInfo first, TrackInfo second);
    void setDirection(bool forwards);

protected:
    int direction;
};

class TitleComparator : public TrackInfoComparator {
public:
    using TrackInfoComparator::TrackInfoComparator;
    ~TitleComparator() = default;

    int compare(TrackInfo first, TrackInfo second) const override;
};

class AlbumComparator : public TrackInfoComparator {
public:
    AlbumComparator(bool forwards);
    ~AlbumComparator() = default;

    int compare(TrackInfo first, TrackInfo second) const override;

    void setTrackComparatorDirection(bool forwards);

private:
    TitleComparator titleComparator;
};

class ArtistComparator : public TrackInfoComparator {
public:
    ArtistComparator(bool forwards);
    ~ArtistComparator() = default;

    int compare(TrackInfo first, TrackInfo second) const override;

private:
    AlbumComparator albumComparator;
};

class LengthComparator : public TrackInfoComparator {
public:
    using TrackInfoComparator::TrackInfoComparator;
    ~LengthComparator() = default;

    int compare(TrackInfo first, TrackInfo second) const override;
};

class TrackComparatorFactory {
public:
    static TrackInfoComparator* getTrackComparator(std::string attribute, bool forwards);
};

class TrackSorter {
public:
    static void sort(std::vector<TrackInfo>& tracks,
                     std::string attribute,
                     bool forwards);
};
