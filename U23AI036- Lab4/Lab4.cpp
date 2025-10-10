
// Substitution Cipher Skeleton Code


#include <bits/stdc++.h>
using namespace std;

// Function to convert a character to uppercase
char toUpper(char c) {
    return toupper(c);
}

// Generate random substitution key
string generateKey() {
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    random_shuffle(alphabet.begin(), alphabet.end());
    return alphabet;
}

// Encrypt function
string encrypt(string text, string key) {
    string ciphertext = "";
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (char p : text) {
        if (isalpha(p)) {
            char upper_p = toUpper(p);
            size_t found = alphabet.find(upper_p);
            if (found != string::npos) {
                char encrypted_char = key[found];
                if (islower(p)) { // Preserve original case
                    ciphertext += tolower(encrypted_char);
                } else {
                    ciphertext += encrypted_char;
                }
            }
        } else {
            ciphertext += p; // Append non-alphabetic characters directly
        }
    }
    return ciphertext;
}

// Decrypt function
string decrypt(string cipher, string key) {
    string plaintext = "";
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (char c : cipher) {
        if (isalpha(c)) {
            char upper_c = toUpper(c);
            size_t found = key.find(upper_c);
            if (found != string::npos) {
                char decrypted_char = alphabet[found];
                if (islower(c)) { // Preserve original case
                    plaintext += tolower(decrypted_char);
                } else {
                    plaintext += decrypted_char;
                }
            }
        } else {
            plaintext += c; // Append non-alphabetic characters directly
        }
    }
    return plaintext;
}

int main() {
    // Seed the random number generator
    srand(time(0));

    string key = generateKey();
    cout << "Key mapping:" << endl;
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < alphabet.length(); ++i) {
        cout << alphabet[i] << "->" << key[i] << endl;
    }
    cout << endl;

    string plaintext;
    cout << "Enter plaintext: ";
    getline(cin, plaintext);

    string cipher = encrypt(plaintext, key);
    cout << "Cipher: " << cipher << endl;

    string decrypted = decrypt(cipher, key);
    cout << "Decrypted: " << decrypted << endl;

    // Optional: Write to files as per lab task
    ofstream cipherFile("cipher.txt");
    if (cipherFile.is_open()) {
        cipherFile << cipher;
        cipherFile.close();
        cout << "Ciphertext written to cipher.txt." << endl;
    } else {
        cout << "Unable to open cipher.txt" << endl;
    }

    ofstream decryptedFile("decrypted.txt");
    if (decryptedFile.is_open()) {
        decryptedFile << decrypted;
        decryptedFile.close();
        cout << "Decrypted text written to decrypted.txt." << endl;
    } else {
        cout << "Unable to open decrypted.txt" << endl;
    }

    return 0;
}