#include <iostream>
#include <random>
#include <cstdint>
#include <iomanip>

using namespace std;

// Helper: modular multiplication (to prevent overflow on 64-bit)
uint64_t modmul(uint64_t a, uint64_t b, uint64_t mod) {
    uint64_t result = 0;
    a %= mod;
    while (b > 0) {
        if (b & 1)
            result = (result + a) % mod;
        a = (2 * a) % mod;
        b >>= 1;
    }
    return result;
}

// Modular exponentiation using repeated squaring
uint64_t modexp(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = modmul(result, base, mod);
        base = modmul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

int main() {
    const uint64_t p = 4294967311ULL;  // public prime
    const uint64_t g = 5ULL;           // generator

    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<uint64_t> dist(2, p - 2);

    uint64_t a = dist(gen); // Alice's private key
    uint64_t b = dist(gen); // Bob's private key

    uint64_t A = modexp(g, a, p);
    uint64_t B = modexp(g, b, p);

    uint64_t KA = modexp(B, a, p);
    uint64_t KB = modexp(A, b, p);

    cout << "Public prime p = " << p << endl;
    cout << "Generator g = " << g << endl;
    cout << "Alice private a = " << a << endl;
    cout << "Alice public A = " << A << endl;
    cout << "Bob private b = " << b << endl;
    cout << "Bob public B = " << B << endl;
    cout << "Shared key computed by Alice = " << KA << endl;
    cout << "Shared key computed by Bob = " << KB << endl;
    cout << "Keys match: " << (KA == KB ? "YES" : "NO") << endl;

    return 0;
}
