#pragma once

#include <juce_core/juce_core.h>

/**
 * @brief List of action messages that can be sent or received.
 *
 */
class ActionMessages {
public:
    inline static const juce::String loadSelectedTracks = "LOAD SELECTED TRACKS";
    inline static const juce::String playTrack = "PLAY TRACK";
    inline static const juce::String queueTrack = "QUEUE TRACK";
    inline static const juce::String pauseTrack = "PAUSE TRACK";
    inline static const juce::String stopTrack = "STOP TRACK";
    inline static const juce::String libraryUpdated = "LIBRARY UPDATED";
    inline static const juce::String playQueueUpdated = "PLAY QUEUE UPDATED";
    inline static const juce::String filesForLibrary = "FILES FOR LIBRARY";
    inline static const juce::String viewLibrary = "VIEW LIBRARY";
    inline static const juce::String viewPlayQueue = "VIEW PLAYQUEUE";
};
