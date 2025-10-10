# Affine Cipher (Lab 7)

### Features
- **Encryption** (`--mode enc`) with key `(a,b)`.
- **Decryption** (`--mode dec`) with key `(a,b)`.
- **Cryptanalysis** (`--mode crack`) using frequency analysis and two-letter mapping.

### Build
```bash
g++ -O2 -std=c++17 affine.cpp -o affine
```

### Usage
```bash
./affine --mode enc --a 5 --b 8 --in "HELLOWORLD"
./affine --mode dec --a 5 --b 8 --in "RCLLAOAPLX"
./affine --mode crack --in "RCLLAOAPLX"
```

### Test Vectors
| (a,b) | Plaintext     | Ciphertext   |
|-------|--------------|--------------|
| (5,8) | HELLOWORLD   | RCLLAOAPLX   |
| (7,3) | CRYPTOGRAPHY | RSPEGXTSDEAP |

### Example Outputs
```bash
$ ./affine --mode enc --a 5 --b 8 --in "HELLOWORLD"
RCLLAOAPLX

$ ./affine --mode dec --a 5 --b 8 --in "RCLLAOAPLX"
HELLOWORLD

$ ./affine --mode crack --in "RCLLAOAPLX"
Ciphertext: RCLLAOAPLX
Candidate (a=5, b=8): HELLOWORLD
```
