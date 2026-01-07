#pragma once

#include <juce_core/juce_core.h>

/**
 * @brief List of action messages that can be sent or received.
 *
 */
class ActionMessages {
public:
    inline static const juce::String loadSelectedTracks = "0";
    inline static const juce::String playTrack = "1";
    inline static const juce::String queueTrack = "2";
    inline static const juce::String pauseTrack = "3";
    inline static const juce::String stopTrack = "4";
    inline static const juce::String nextTrack = "5";
    inline static const juce::String restartTrack = "6";
    inline static const juce::String playAlbum = "7";
    inline static const juce::String playArtist = "8";
    inline static const juce::String libraryUpdated = "9";
    inline static const juce::String playQueueUpdated = "10";
    inline static const juce::String filesForLibrary = "11";
    inline static const juce::String viewLibrary = "12";
    inline static const juce::String viewPlayQueue = "13";
    inline static const juce::String removeTracksFromLibrary = "14";
    inline static const juce::String deleteTracksFromHarddrive = "15";
    inline static const juce::String shuffleModeChanged = "16";
    inline static const juce::String playRandomAlbum = "17";
    inline static const juce::String removeSelectedFromPlayQueue = "18";
};
