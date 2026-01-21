#pragma once
#include <string>

namespace utilities {

/**
 * @brief Write lowercase version of `src` to `dst`.
 *
 * @param src
 * @param dst
 */
void toLower(std::string& src, std::string& dst);

/**
 * @brief Format the given number of seconds into 'mm:ss' format.
 *
 * @param seconds
 * @return std::string
 */
std::string formatSeconds(double seconds);

}  // namespace utilities
