This project implements a toy version of the RSA cryptosystem in a single file (q1.cpp), including key generation, encryption, and decryption using small primes.

Files
q1.cpp → Self-contained implementation and test driver.

Compilation

Using g++:
g++ -std=c++11 q1.cpp -o q1

Expected Output
RSA Implementation (Toy Example)
p = 61
q = 53
n = 3233
phi(n) = 3120
Public exponent e = 17
Private exponent d = 2753

Message m = 65
Ciphertext c = 2790
Decrypted message m_dec = 65

[OK] Decryption successful!