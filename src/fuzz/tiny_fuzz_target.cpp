#include <stdint.h>
#include <stddef.h>
#include <string>
#include <vector>
#include <cstring>

// Include our library to fuzz
#include "../lib/tiny_lib.h"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Avoid processing extremely large inputs
    if (size > 10000) {
        return 0;
    }

    // Test 1: Parse number from the input data as a string
    if (size > 0) {
        std::string input_str(reinterpret_cast<const char*>(data), size);
        
        // Exercise the parse_number function
        int result = TinyLib::parse_number(input_str);
        
        // Force some branches by checking the result
        if (result == -1) {
            // Empty string case
        } else if (result == -2) {
            // Too long case
        } else if (result == -3) {
            // Invalid format case
        } else if (result > 1000000) {
            // Large number case - this could trigger interesting behavior
        } else if (result < -1000000) {
            // Large negative number case
        }
    }

    // Test 2: Check ASCII validation
    if (size > 0) {
        std::string ascii_test(reinterpret_cast<const char*>(data), size);
        bool is_ascii = TinyLib::is_valid_ascii(ascii_test);
        
        // Exercise both branches
        if (is_ascii) {
            // Valid ASCII path
        } else {
            // Invalid ASCII path - might contain high-bit characters
        }
    }

    // Test 3: Process raw data
    int checksum = TinyLib::process_data(data, size);
    
    // Create some interesting edge cases to exercise different code paths
    if (size >= 4) {
        // Test with different data sizes
        TinyLib::process_data(data, 1);
        TinyLib::process_data(data, size / 2);
        TinyLib::process_data(data, size - 1);
    }

    // Test 4: Exercise edge cases with specific patterns
    if (size >= 10) {
        // Try to create some patterns that might trigger edge cases
        std::vector<uint8_t> pattern_data(data, data + 10);
        
        // Test with null bytes
        for (auto& byte : pattern_data) {
            if (byte == 0) {
                std::string null_test(reinterpret_cast<const char*>(pattern_data.data()), 10);
                TinyLib::parse_number(null_test);
                break;
            }
        }
        
        // Test with very long repeated patterns
        if (size >= 50) {
            std::string long_pattern;
            for (size_t i = 0; i < std::min(size_t(200), size); ++i) {
                long_pattern += static_cast<char>(data[i % size]);
            }
            TinyLib::parse_number(long_pattern);
        }
    }

    // Test 5: Exercise boundary conditions
    if (size == 1) {
        char single_char[2] = {static_cast<char>(data[0]), '\0'};
        TinyLib::parse_number(std::string(single_char));
    }

    // Test 6: Try to trigger integer parsing edge cases
    if (size >= 8) {
        // Create potential integer strings
        std::string potential_int;
        for (size_t i = 0; i < std::min(size_t(20), size); ++i) {
            char c = data[i];
            if (c >= '0' && c <= '9') {
                potential_int += c;
            } else if (c == '-' || c == '+') {
                potential_int += c;
            } else {
                break;
            }
        }
        if (!potential_int.empty()) {
            TinyLib::parse_number(potential_int);
        }
    }

    return 0;  // Return 0 as per LibFuzzer convention
}
