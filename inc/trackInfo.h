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
          path(path) {
        normalizeNames();
    }

    /**
     * @brief Format track info into a string.
     *
     * @return std::string
     */
    std::string toString() const;

    // Temporary
    void setMetadata(juce::StringPairArray data) { metadata = data; }

    std::string getArtist() const { return artist; }
    std::string getNormalizedArtist() const { return normalizedNames.artist; }
    std::string getAlbum() const { return album; }
    std::string getNormalizedAlbum() const { return normalizedNames.album; }
    std::string getTitle() const { return title; }
    std::string getNormalizedTitle() const { return normalizedNames.title; }
    juce::File getPath() const { return path; }
    juce::StringPairArray getMetadata() const { return metadata; }

    void setArtist(std::string newArtist) {
        artist = newArtist;
        normalizeArtist();
    }
    void setAlbum(std::string newAlbum) {
        album = newAlbum;
        normalizeAlbum();
    }
    void setTitle(std::string newTitle) {
        title = newTitle;
        normalizeTitle();
    }
    void setPath(juce::File newPath) { path = newPath; }
    void setLength(double newLength) { lengthInSeconds = newLength; }

    /**
     * @brief Track length in seconds.
     *
     * @return double
     */
    double getLength() { return lengthInSeconds; }

    std::string getLengthString() const;

    bool operator==(const TrackInfo& track);
    bool operator==(TrackInfo& track);

private:
    std::string artist;
    std::string album;
    std::string title;
    double lengthInSeconds;
    juce::File path;
    juce::StringPairArray metadata;
    // Want to ignore case when sorting
    // so use this to cache lowercase names
    struct NormalizedNames {
        std::string artist;
        std::string album;
        std::string title;
    } normalizedNames;

    void normalizeArtist();
    void normalizeAlbum();
    void normalizeTitle();
    void normalizeNames();
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
    /**
     * @brief Sorts the given tracks in-place.
     *
     * @param tracks
     * @param attribute The attribute to sort on ('Title', 'Album', 'Artist', 'Length').
     * @param forwards Whether to sort forwards or backwards.
     */
    static void sort(std::vector<TrackInfo>& tracks,
                     std::string attribute,
                     bool forwards);
};
