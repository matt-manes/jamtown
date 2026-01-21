#include "utilities.h"
#include <algorithm>
#include <format>

void utilities::toLower(std::string& src, std::string& dst) {
    dst.resize(src.size());
    std::transform(src.begin(), src.end(), dst.begin(), [](unsigned char c) {
        return static_cast<unsigned char>(std::tolower(c));
    });
}

std::string utilities::formatSeconds(double seconds) {
    int m = static_cast<int>(static_cast<int>(seconds / 60) % 60);
    int s = static_cast<int>(static_cast<int>(seconds) % 60);
    if (s == 0)
        return std::format("{0}:00", m);
    else if (s < 10)
        return std::format("{0}:0{1}", m, s);
    else
        return std::format("{0}:{1}", m, s);
}
