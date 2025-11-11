#include <bits/stdc++.h>
using namespace std;

struct EG { long long g, x, y; };

EG egcd(long long a, long long b) {
    if (b == 0) return {a, 1, 0};
    EG t = egcd(b, a % b);
    return {t.g, t.y, t.x - (a / b) * t.y};
}

long long modinv(long long a, long long m) {
    EG t = egcd(a, m);
    if (t.g != 1) {
        cerr << "No modular inverse exists!\n";
        return -1;
    }
    long long res = t.x % m;
    if (res < 0) res += m;
    return res;
}

long long modexp(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp /= 2;
    }
    return res;
}

int main() {
    cout << "RSA Implementation (Toy Example)\n";

    long long p = 61, q = 53;
    long long n = p * q;
    long long phi = (p - 1) * (q - 1);
    long long e = 17;
    long long d = modinv(e, phi);

    cout << "p = " << p << "\nq = " << q << "\n";
    cout << "n = " << n << "\nphi(n) = " << phi << "\n";
    cout << "Public exponent e = " << e << "\nPrivate exponent d = " << d << "\n";

    long long m = 65;
    long long c = modexp(m, e, n);
    long long m_dec = modexp(c, d, n);

    cout << "\nMessage m = " << m;
    cout << "\nCiphertext c = " << c;
    cout << "\nDecrypted message m_dec = " << m_dec << "\n";

    if (m == m_dec) cout << "\n[OK] Decryption successful!\n";
    else cout << "\n[ERROR] Decryption failed!\n";

    return 0;
}
