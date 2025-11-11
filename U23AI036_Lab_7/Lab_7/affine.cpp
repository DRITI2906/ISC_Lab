/* 
AI331 Lab 7: Affine Cipher (C++ Implementation + Cryptanalysis)
---------------------------------------------------------------
Build:
  g++ -O2 -std=c++17 affine.cpp -o affine

Usage:
  Encryption: ./affine --mode enc --a <int> --b <int> --in "<plaintext>"
  Decryption: ./affine --mode dec --a <int> --b <int> --in "<ciphertext>"
  Cryptanalysis: ./affine --mode crack --in "<ciphertext>"

Policy:
  - Converts to uppercase A..Z
  - Drops non-letters
*/

#include <bits/stdc++.h>
using namespace std;

// ---------- helpers ----------
static inline int mod(int x, int m) {
    int r = x % m;
    return (r < 0) ? r + m : r;
}

static inline bool is_valid_a(int a) {
    int x = abs(a), y = 26;
    while (y) {
        int t = x % y;
        x = y;
        y = t;
    }
    return x == 1;
}

static int modinv(int a, int m) {
    int r0 = a, r1 = m;
    int s0 = 1, s1 = 0;
    while (r1 != 0) {
        int q = r0 / r1;
        int r2 = r0 - q * r1;
        int s2 = s0 - q * s1;
        r0 = r1; r1 = r2;
        s0 = s1; s1 = s2;
    }
    if (r0 != 1) throw runtime_error("modinv: no inverse");
    return mod(s0, m);
}

static string sanitize(const string& s) {
    string t;
    for (char c : s)
        if (isalpha((unsigned char)c))
            t.push_back((char)toupper(c));
    return t;
}

// ---------- core cipher ----------
static string encrypt_text(const string& plain, int a, int b) {
    string out;
    out.reserve(plain.size());
    for (char c : plain) {
        int x = c - 'A';
        int y = (a * x + b) % 26;
        out.push_back('A' + y);
    }
    return out;
}

static string decrypt_text(const string& cipher, int a, int b) {
    string out;
    out.reserve(cipher.size());
    int a_inv = modinv(a, 26);
    for (char c : cipher) {
        int y = c - 'A';
        int x = mod(a_inv * (y - b), 26);
        out.push_back('A' + x);
    }
    return out;
}

// ---------- frequency analysis ----------
pair<char,char> top_two_letters(const string& s) {
    vector<int> freq(26, 0);
    for (char c : s)
        if (isalpha((unsigned char)c)) freq[toupper(c) - 'A']++;
    vector<pair<int,char>> sorted;
    for (int i = 0; i < 26; i++)
        sorted.push_back({freq[i], 'A' + i});
    sort(sorted.rbegin(), sorted.rend());
    return {sorted[0].second, sorted[1].second};
}

bool solve_keys(char x1c, char y1c, char x2c, char y2c, int &a, int &b) {
    int x1 = x1c - 'A', y1 = y1c - 'A';
    int x2 = x2c - 'A', y2 = y2c - 'A';
    if (x1 == x2) return false;
    int num = (y1 - y2 + 26) % 26;
    int den = (x1 - x2 + 26) % 26;
    if (!is_valid_a(den)) return false;
    int den_inv = modinv(den, 26);
    a = (num * den_inv) % 26;
    if (!is_valid_a(a)) return false;
    b = (y1 - a * x1) % 26;
    b = mod(b, 26);
    return true;
}

void crack_mode(const string& cipher) {
    string in = sanitize(cipher);
    if (in.empty()) {
        cerr << "Empty ciphertext!\n";
        return;
    }
   auto result = top_two_letters(in);
   auto ymax = result.first;
   auto y2nd = result.second;

   vector<pair<char, char>> pairs = {
       {'E', ymax}, {'T', y2nd},
       {'E', y2nd}, {'T', ymax}
    };

    cout << "Ciphertext: " << in << "\n";
    for (size_t i = 0; i < pairs.size(); i += 2) {
        int a, b;
        if (solve_keys(pairs[i].first, pairs[i].second,
                       pairs[i+1].first, pairs[i+1].second, a, b)) {
            try {
                string plain = decrypt_text(in, a, b);
                cout << "Candidate (a=" << a << ", b=" << b << "): "
                     << plain << "\n";
            } catch (...) {}
        }
    }
}

// ---------- main ----------
int main(int argc, char** argv) {
    string mode, text;
    int a = -1, b = 0;
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--mode" && i + 1 < argc) mode = argv[++i];
        else if (arg == "--a" && i + 1 < argc) a = stoi(argv[++i]);
        else if (arg == "--b" && i + 1 < argc) b = stoi(argv[++i]);
        else if (arg == "--in" && i + 1 < argc) text = argv[++i];
    }
    if (mode.empty()) {
        cerr << "Usage:\n"
             << "  ./affine --mode enc --a <int> --b <int> --in \"<text>\"\n"
             << "  ./affine --mode dec --a <int> --b <int> --in \"<text>\"\n"
             << "  ./affine --mode crack --in \"<ciphertext>\"\n";
        return 1;
    }
    text = sanitize(text);
    try {
        if (mode == "enc") {
            if (!is_valid_a(a)) {
                cerr << "Error: invalid a. Must be coprime with 26.\n"; return 1;
            }
            cout << encrypt_text(text, a, b) << "\n";
        } else if (mode == "dec") {
            if (!is_valid_a(a)) {
                cerr << "Error: invalid a. Must be coprime with 26.\n"; return 1;
            }
            cout << decrypt_text(text, a, b) << "\n";
        } else if (mode == "crack") {
            crack_mode(text);
        } else {
            cerr << "Unknown mode.\n"; return 1;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n"; return 1;
    }
    return 0;
}
