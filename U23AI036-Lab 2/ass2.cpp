#include <iostream>
#include <fstream>
#include <string>
using namespace std;

char encryptChar(char c) {
    if (isalpha(c)) {
        char base = islower(c) ? 'a' : 'A';
        return (c - base + 3) % 26 + base;
    }
    return c;
}

void encryptFile(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile);
    ofstream out(outputFile);
    string line;

    if (!in || !out) {
        cerr << "File error!\n";
        return;
    }

    while (getline(in, line)) {
        for (char c : line) {
            out << encryptChar(c);
        }
        out << '\n';
    }

    in.close();
    out.close();
    cout << "Encryption done. Output saved to " << outputFile << endl;
}

char decryptChar(char c) {
    if (isalpha(c)) {
        char base = islower(c) ? 'a' : 'A';
        return (c - base - 3 + 26) % 26 + base;  // +26 handles negative mod
    }
    return c;
}

void decryptFile(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile);
    ofstream out(outputFile);
    string line;

    if (!in || !out) {
        cerr << "File error!\n";
        return;
    }

    while (getline(in, line)) {
        for (char c : line) {
            out << decryptChar(c);
        }
        out << '\n';
    }

    in.close();
    out.close();
    cout << "Decryption done. Output saved to " << outputFile << endl;
}

int main() {
    string inputPlain = "plaintext.txt";
    string encrypted = "ciphertext.txt";
    string decrypted = "decrypted.txt";

    encryptFile(inputPlain, encrypted);
    decryptFile(encrypted, decrypted);

    return 0;
}