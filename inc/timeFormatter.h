#pragma once

#include <format>

inline std::string formatSeconds(double seconds) {
    int m = static_cast<int>(static_cast<int>(seconds / 60) % 60);
    int s = static_cast<int>(static_cast<int>(seconds) % 60);
    if (s == 0)
        return std::format("{0}:00", m);
    else if (s < 10)
        return std::format("{0}:0{1}", m, s);
    else
        return std::format("{0}:{1}", m, s);
}
