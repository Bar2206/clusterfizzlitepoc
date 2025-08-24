#pragma once

#include <string>

/**
 * A tiny library for ClusterFuzzLite demonstration.
 * Provides basic number parsing functionality for fuzzing.
 */
class TinyLib {
public:
    /**
     * Parse a string to an integer.
     * Returns the parsed integer, or sentinel values on error:
     * - Returns -1 for empty strings
     * - Returns -2 for strings that are too long (>100 chars)
     * - Returns -3 for invalid numeric strings
     * - Returns the parsed value for valid integers
     */
    static int parse_number(const std::string& input);

    /**
     * Validate if a string contains only ASCII characters.
     * Returns true if valid ASCII, false otherwise.
     */
    static bool is_valid_ascii(const std::string& input);

    /**
     * Process a buffer of data (useful for fuzzing).
     * Returns a simple checksum of the data.
     */
    static int process_data(const uint8_t* data, size_t size);
};
