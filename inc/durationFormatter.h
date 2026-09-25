#pragma once

#include <array>
#include <cstdint>
#include <string>

/**
 * @brief Formats a given number of seconds into a `y m w d h min s` format, omitting units that are zero.
 *
 */
class DurationFormatter {
public:
    static std::string formatDuration(std::uint64_t seconds);

private:
    using UnitValues = std::array<std::uint64_t, 7>;

    DurationFormatter() = delete;

    static const std::array<std::uint64_t, 7> unitSizes;
    static const std::array<const char*, 7> units;
};
