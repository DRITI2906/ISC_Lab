# Lab 10: Implementation of Diffie–Hellman Key Exchange

## Implementation Details
- Language: **C++17**
- Prime number: **p = 4294967311**
- Generator: **g = 5**
- Random private keys `a` (Alice) and `b` (Bob)
- Modular exponentiation implemented via **repeated squaring**
- Modular multiplication used to prevent overflow on 64-bit systems

### Files
- `q1.cpp` — Source code
- `output.txt` — Sample program output

### Compilation & Execution
g++ -std=c++17 q1.cpp -o q1
./q1 > output.txt


### Task 4:
The Diffie–Hellman key exchange is secure because both parties compute a shared secret using modular exponentiation, while their private keys remain unknown to outsiders. Even if an attacker observes all public values p,g,A,B, they cannot determine the private exponents a or b. Recovering them would require solving the Discrete Logarithm Problem (DLP) — finding a given A =  (g^a) modp, which is computationally infeasible for large primes. Thus, the shared key (g^ab) modp remains secure even over an insecure communication channel.