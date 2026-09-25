#include "DurationFormatter.h"

#include <sstream>
#include <string>
#include <format>

const std::array<std::uint64_t, 7> DurationFormatter::unitSizes{
    365ULL * 24 * 60 * 60,  // year
    30ULL * 24 * 60 * 60,   // month
    7ULL * 24 * 60 * 60,    // week
    24ULL * 60 * 60,        // day
    60ULL * 60,             // hour
    60,                     // minute
    1                       // second
};

const std::array<const char*, 7> DurationFormatter::units{
    "y", "m", "w", "d", "h", "min", "s"};

std::string DurationFormatter::formatDuration(std::uint64_t seconds) {
    if (seconds == 0)
        return std::format("0{}", units.back());

    std::ostringstream out;
    std::uint64_t value;
    for (std::size_t i = 0; i < unitSizes.size(); ++i) {
        value = seconds / unitSizes[i];
        seconds %= unitSizes[i];
        if (value > 0)
            out << value << units[i] << ' ';
    }

    std::string result = out.str();
    // Remove last space
    result.pop_back();
    return result;
}
