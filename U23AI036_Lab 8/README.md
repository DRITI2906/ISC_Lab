# DES Round Function Implementation


**Name:** Driti Rathod
**Roll No:** U23AI036

## Overview
This project implements the DES round function `f` for the Information Security & Cryptography lab assignment.

## Files
* `des.hpp` - Header file with function declarations
* `des.cpp` - Implementation of the Feistel round function
* `des_tables.cpp` - Provided tables and key schedule (not modified)
* `test_main.cpp` - Test program to verify implementation

## Implementation Details
The `feistel()` function implements the DES round function in four steps:

1. **E-Expansion**: Expands 32-bit input R to 48 bits using the E-expansion table
2. **XOR Operation**: XORs the expanded value with the 48-bit subkey
3. **S-box Substitution**: Applies 8 S-boxes, each converting 6 bits to 4 bits
   * Row index: formed by outer bits (bit 5 and bit 0)
   * Column index: formed by middle 4 bits (bits 4-1)
4. **P-Permutation**: Applies final permutation to produce 32-bit output

## Compilation

```bash
g++ -std=c++11 -o des_test test_main.cpp des.cpp des_tables.cpp
```

## Test Results

### Test Case
* **Input R**: `0xF0AAF0AA`
* **Subkey K**: `0x1B02EFFC7072`
* **Expected Output**: `0x234AA9BB`
* **Actual Output**: `0x234AA9BB`
* **Status**: ✓ PASSED

### Key Schedule Output
For key `0x133457799BBCDFF1`:

```
Round  1: 0x1B02EFFC7072
Round  2: 0x79AED9DBC9E5
Round  3: 0x55FC8A06D1FA
Round  4: 0x72ADD6DB351D
Round  5: 0x7CEC07EB53A8
Round  6: 0x63A53E507B2F
Round  7: 0xEC84B7F618BC
Round  8: 0xF75E369A2D9F
Round  9: 0xDB4F1C64763C
Round 10: 0x81AC77D6ECF2
Round 11: 0x5F6A1D2F573E
Round 12: 0x0DC98C1AEDA7
Round 13: 0x33638D0CBFD0
Round 14: 0x196A0B2C95F5
Round 15: 0x42C7F0EDE5E0
Round 16: 0x7A9BE42F2009
```

## Notes
* The implementation uses 1-based bit numbering as per DES specification
* S-box lookups use row/column indexing where row is formed by outer bits
* All permutations read from MSB to LSB
* The implementation matches the expected test output exactly