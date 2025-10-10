#include "des.hpp"
#include <iostream>
#include <iomanip>
#include <cstdint>

int main() {
    std::cout << "=== DES Round Function Test ===\n\n";
    
    // Test case from the lab document
    uint32_t R = 0xF0AAF0AA;
    uint64_t K = 0x1B02EFFC7072;
    uint32_t expected = 0x234AA9BB;
    
    std::cout << "Input R:       0x" << std::hex << std::uppercase 
              << std::setw(8) << std::setfill('0') << R << "\n";
    std::cout << "Subkey K:      0x" << std::setw(12) << K << "\n";
    std::cout << "Expected:      0x" << std::setw(8) << expected << "\n";
    
    uint32_t result = des::feistel(R, K);
    
    std::cout << "Actual result: 0x" << std::setw(8) << result << "\n";
    
    if (result == expected) {
        std::cout << "\n✓ TEST PASSED!\n";
    } else {
        std::cout << "\n✗ TEST FAILED!\n";
    }
    
    // Optional: Display key schedule
    std::cout << "\n=== Key Schedule ===\n";
    uint64_t key = 0x133457799BBCDFF1ULL;
    uint64_t subkeys[16];
    des::key_schedule(key, subkeys);
    
    std::cout << "Key: 0x" << std::hex << std::uppercase << key << "\n\n";
    for (int i = 0; i < 16; ++i) {
        std::cout << "Round " << std::dec << std::setw(2) << (i+1)
                  << ": 0x" << std::hex << std::uppercase
                  << std::setw(12) << std::setfill('0') << subkeys[i] << "\n";
    }
    
    return 0;
}