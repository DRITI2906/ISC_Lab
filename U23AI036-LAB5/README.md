# Playfair Cipher Implementation

This project implements the *Playfair Cipher* in C++.  
It supports:
- Building the key square from a given keyword
- Encryption of plaintext messages
- Decryption of ciphertext back to plaintext
- Handling repeated letters and padding (X)
- Running multiple *test vectors* automatically
- Writing all results into results.txt

---

## How It Works
1. *Key Square Generation*
   - Input keyword is normalized (A–Z, J→I).
   - Duplicate letters removed.
   - Remaining alphabet filled in (excluding J).
   - Square is 5×5.

2. *Digraph Processing*
   - Plaintext is split into pairs.
   - If both letters are the same → insert X between them.
   - If odd length → pad with X.

3. *Encryption Rules*
   - Same row → replace with letters to the right.
   - Same column → replace with letters below.
   - Rectangle → swap corners.

4. *Decryption Rules*
   - Same row → replace with letters to the left.
   - Same column → replace with letters above.
   - Rectangle → swap corners.

---

## Files
- ass5.cpp → Playfair cipher implementation
- results.txt → Generated file with all test outputs
- README.md → Project documentation (this file)

---

## Compilation & Run
```bash
g++ main.cpp -o ass5
./ass5