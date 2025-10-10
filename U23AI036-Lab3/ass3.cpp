#include <iostream>
#include <fstream>
#include <random>
using namespace std;

// Read file into byte vector
vector<unsigned char> readFile(const string &filename) {
    ifstream file(filename, ios::binary);
    if (!file) throw runtime_error("Error opening file: " + filename);
    return vector<unsigned char>((istreambuf_iterator<char>(file)), {});
}

// Write byte vector to file
void writeFile(const string &filename, const vector<unsigned char> &data) {
    ofstream file(filename, ios::binary);
    if (!file) throw runtime_error("Error writing file: " + filename);
    file.write(reinterpret_cast<const char *>(data.data()), data.size());
}

// Generate truly random key
vector<unsigned char> generateKey(size_t length) {
    vector<unsigned char> key(length);
    random_device rd; // Non-deterministic hardware RNG
    for (size_t i = 0; i < length; i++) {
        key[i] = static_cast<unsigned char>(rd());
    }
    return key;
}

// XOR two byte arrays
vector<unsigned char> xorData(const vector<unsigned char> &a, const vector<unsigned char> &b) {
    if (a.size() != b.size()) throw runtime_error("Data length mismatch");
    vector<unsigned char> result(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        result[i] = a[i] ^ b[i];
    }
    return result;
}

int main() {
    try {
        // --- Encryption ---
        auto plaintext = readFile("image.png");
        auto key = generateKey(plaintext.size());
        writeFile("key.bin", key);

        auto ciphertext = xorData(plaintext, key);
        writeFile("ciphertext.bin", ciphertext);

        // --- Decryption ---
        auto readKey = readFile("key.bin");
        auto decrypted = xorData(ciphertext, readKey);
        writeFile("decrypted.png", decrypted);

        cout << "[+] Encryption & Decryption completed.\n";

        // --- Key Reuse Attack Demo ---
        string m1 = "HELLO";
        string m2 = "WORLD";
        vector<unsigned char> M1(m1.begin(), m1.end());
        vector<unsigned char> M2(m2.begin(), m2.end());

        auto reuseKey = generateKey(M1.size());
        auto C1 = xorData(M1, reuseKey);
        auto C2 = xorData(M2, reuseKey);

        auto leaked = xorData(C1, C2); // C1 ⊕ C2 = M1 ⊕ M2

        cout << "[!] Key reuse attack demonstration:\n";
        cout << "M1 ⊕ M2 (leaked): ";
        for (auto ch : leaked) cout << (char)ch;

    } catch (const exception &e) {
        cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}