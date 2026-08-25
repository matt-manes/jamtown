#pragma once

#include <vector>
#include <string>
#include <exception>
#include "trackInfo.h"
#include <juce_core/juce_core.h>
#include "libraryPersistanceService.h"

class LibFileException : public std::exception {
public:
    LibFileException(const std::string msg) : message(msg) {}

    const char* what() const noexcept { return message.c_str(); }

private:
    std::string message;
};

/**
 * @brief Implementation of `LibraryPersistanceService` that reads and writes `TrackInfo`
 * objects using a simple format saved to a text file.
 *
 * The format is:
 * @code
 *  Title\n
 *  Album\n
 *  Artist\n
 *  TrackLengthInSeconds\n
 *  FullFilePath\n
 *  MetadataKey1:MetadataValue1\n
 *  MetadataKeyn:MetadataValuen\n
 *  STOP\n
 * @endcode
 *
 * The number of metadata rows is arbitrary and can be 0.
 * The sequence repeats for each additional track.
 */
class TxtLibraryPersistanceService : public LibraryPersistanceService {
public:
    TxtLibraryPersistanceService();

    void save(std::vector<TrackInfo> tracks) override;

    std::vector<TrackInfo> load() override;

private:
    std::string relativeLibPath;

    juce::File getLibFile();

    /**
     * @brief Format the given track into a writeable string.
     *
     * @param track
     * @return std::string
     */
    std::string prepareTrackInfoForSave(TrackInfo track);
};
