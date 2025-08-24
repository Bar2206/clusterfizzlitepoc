#include "tiny_lib.h"
#include <stdexcept>
#include <climits>
#include <cctype>

int TinyLib::parse_number(const std::string& input) {
    // Handle empty string
    if (input.empty()) {
        return -1;
    }
    
    // Handle overly long strings (potential DoS vector)
    if (input.length() > 100) {
        return -2;
    }
    
    try {
        // Check for non-numeric characters (except leading +/- and whitespace)
        size_t start = 0;
        while (start < input.length() && std::isspace(input[start])) {
            start++;
        }
        
        if (start >= input.length()) {
            return -3; // Only whitespace
        }
        
        // Allow leading sign
        if (input[start] == '+' || input[start] == '-') {
            start++;
        }
        
        // Check remaining characters are digits
        for (size_t i = start; i < input.length(); ++i) {
            if (!std::isdigit(input[i])) {
                return -3; // Invalid character found
            }
        }
        
        // Convert to integer
        long long value = std::stoll(input);
        
        // Check for overflow
        if (value > INT_MAX || value < INT_MIN) {
            return -3;
        }
        
        return static_cast<int>(value);
        
    } catch (const std::exception&) {
        return -3; // Parsing failed
    }
}

bool TinyLib::is_valid_ascii(const std::string& input) {
    for (char c : input) {
        if (static_cast<unsigned char>(c) > 127) {
            return false;
        }
    }
    return true;
}

int TinyLib::process_data(const uint8_t* data, size_t size) {
    if (!data || size == 0) {
        return 0;
    }
    
    int checksum = 0;
    for (size_t i = 0; i < size; ++i) {
        checksum ^= data[i];
        checksum = (checksum << 1) | ((checksum & 0x80000000) ? 1 : 0); // Rotate left
    }
    
    return checksum;
}
