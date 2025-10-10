#include "des.hpp"
#include <cstdint>
#include <cstddef>

namespace des {

// --- Key schedule tables ---
// PC-1: 64 -> 56-bit key (drop parity)
static const int PC1[56] = {
    57,49,41,33,25,17, 9,
     1,58,50,42,34,26,18,
    10, 2,59,51,43,35,27,
    19,11, 3,60,52,44,36,
    63,55,47,39,31,23,15,
     7,62,54,46,38,30,22,
    14, 6,61,53,45,37,29,
    21,13, 5,28,20,12, 4
};

// PC-2: 56 -> 48-bit subkey
static const int PC2[48] = {
    14,17,11,24, 1, 5, 3,28,
    15, 6,21,10,23,19,12, 4,
    26, 8,16, 7,27,20,13, 2,
    41,52,31,37,47,55,30,40,
    51,45,33,48,44,49,39,56,
    34,53,46,42,50,36,29,32
};

// Left-rotation schedule for C,D (28-bit halves)
static const int ROTATIONS[16] = { 1,1,2,2,2,2,2,2, 1,2,2,2,2,2,2,1 };

// Generic permutation for a 64-bit container (MSB-first bit numbering)
static inline uint64_t permute(uint64_t in, const int* table, int n){
    uint64_t out = 0;
    for(int i=0;i<n;++i){
        int from = table[i]-1; // 1-based -> 0-based
        uint64_t bit = (in >> (64-1-from)) & 1ULL; // take bit at position 'from'
        out = (out<<1) | bit;
    }
    return out;
}

static inline uint64_t left_rotate28(uint64_t v, int r){
    v &= 0x0FFFFFFFULL; // 28 bits
    return ((v << r) | (v >> (28 - r))) & 0x0FFFFFFFULL;
}

// Key schedule: input 64-bit key (with parity bits); outputs 16 subkeys (low 48 bits used)
void key_schedule(uint64_t key64, uint64_t subkeys[16]){
    // PC-1: 64 -> 56 (drop parity)
    uint64_t key56 = permute(key64, PC1, 56);
    uint64_t C = (key56 >> 28) & 0x0FFFFFFFULL;
    uint64_t D = key56 & 0x0FFFFFFFULL;
    for(int r=0;r<16;++r){
        C = left_rotate28(C, ROTATIONS[r]);
        D = left_rotate28(D, ROTATIONS[r]);
        uint64_t CD = (C<<28) | D; // 56 bits
        // PC-2: 56 -> 48
        uint64_t k48 = 0;
        for(int i=0;i<48;++i){
            int from = PC2[i]-1;
            uint64_t bit = (CD >> (56-1-from)) & 1ULL;
            k48 = (k48<<1) | bit;
        }
        subkeys[r] = k48; // stored in low 48 bits
    }
}

} // namespace des