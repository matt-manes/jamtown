#include "visibleTracksDataComponent.h"
#include <cstdint>
#include <format>
#include "durationFormatter.h"

void VisibleTracksDataComponent::update(int numTracks, double totalTime) {
    setText(std::format(
                "Tracks: {} - Time: {}",
                numTracks,
                DurationFormatter::formatDuration(static_cast<std::uint64_t>(totalTime))),
            juce::NotificationType::dontSendNotification);
}
