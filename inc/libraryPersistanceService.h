#pragma once

#include <vector>
#include <string>
#include "trackInfo.h"
#include <juce_core/juce_core.h>

/**
 * @brief Abstract class for persisting `TrackInfo` objects.
 *
 */
class LibraryPersistanceService {
public:
    virtual ~LibraryPersistanceService() {}

    virtual void save(std::vector<TrackInfo> tracks) = 0;

    virtual std::vector<TrackInfo> load() = 0;
};
