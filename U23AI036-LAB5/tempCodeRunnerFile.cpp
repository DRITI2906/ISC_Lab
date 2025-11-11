#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <cctype>
#include <stdexcept>

using namespace std;

struct Pos { int r, c; };

// global containers used by functions
array<array<char,5>,5> keySquare;
array<int,26> posIdx; // posIdx[ch - 'A'] -> index 0..24

// Normalize: keep letters only, uppercase, map J -> I
string normalize(const string &in) {
    string out;
    out.reserve(in.size());
    for (char ch : in) {
        if (isalpha((unsigned char)ch)) {
            char u = (char)toupper((unsigned char)ch);
            if (u == 'J') u = 'I';
            out.push_back(u);
        }
    }
    return out;
}

// Build the 5x5 key square from keyword (J treated as I)
void buildKeySquare(const string &keyword) {
    string key = normalize(keyword);

    array<bool,26> used{};
    used.fill(false);
    used['J' - 'A'] = true; // exclude J

    string order;
    order.reserve(25);

    // add unique letters from keyword in order
    for (char c : key) {
        if (!used[c - 'A']) {
            order.push_back(c);
            used[c - 'A'] = true;
        }
    }

    // add remaining alphabet letters A..Z (skip J)
    for (char c = 'A'; c <= 'Z'; ++c) {
        if (c == 'J') continue;
        if (!used[c - 'A']) {
            order.push_back(c);
            used[c - 'A'] = true;
        }
    }

    if ((int)order.size() != 25) {
        throw runtime_error("Internal error while building key square (order size != 25).");
    }

    posIdx.fill(-1);
    for (int i = 0; i < 25; ++i) {
        int r = i / 5;
        int c = i % 5;
        keySquare[r][c] = order[i];
        posIdx[order[i] - 'A'] = i;
    }
}

// Convert normalized plaintext into digraphs following Playfair rules
vector<pair<char,char>> toDigraphs(const string &plaintext) {
    string s = normalize(plaintext);
    vector<pair<char,char>> digs;
    digs.reserve((s.size() + 1) / 2);

    size_t i = 0;
    while (i < s.size()) {
        char a = s[i];
        char b;
        if (i + 1 >= s.size()) {
            // last single char -> pad with X
            b = 'X';
            i += 1;
        } else {
            b = s[i + 1];
            if (a == b) {
                // identical pair -> insert 'X' after a
                b = 'X';
                i += 1; // advance only one
            } else {
                i += 2; // normal pair
            }
        }
        digs.emplace_back(a, b);
    }
    return digs;
}

// Encrypt plaintext (any input; normalization and digraphing handled inside)
string encrypt(const string &plaintext) {
    vector<pair<char,char>> digs = toDigraphs(plaintext);
    string out;
    out.reserve(digs.size() * 2);

    for (const auto &pr : digs) {
        char A = pr.first;
        char B = pr.second;
        int idxA = posIdx[A - 'A'];
        int idxB = posIdx[B - 'A'];
        if (idxA < 0 || idxB < 0) throw runtime_error("Invalid letter (after normalize) in encrypt.");

        int ra = idxA / 5, ca = idxA % 5;
        int rb = idxB / 5, cb = idxB % 5;

        if (ra == rb) {
            // same row -> shift right
            out.push_back(keySquare[ra][(ca + 1) % 5]);
            out.push_back(keySquare[rb][(cb + 1) % 5]);
        } else if (ca == cb) {
            // same column -> shift down
            out.push_back(keySquare[(ra + 1) % 5][ca]);
            out.push_back(keySquare[(rb + 1) % 5][cb]);
        } else {
            // rectangle -> swap columns
            out.push_back(keySquare[ra][cb]);
            out.push_back(keySquare[rb][ca]);
        }
    }
    return out;
}

// Decrypt ciphertext (assumes even-length valid Playfair ciphertext)
string decrypt(const string &ciphertext) {
    string s = normalize(ciphertext);
    if (s.size() % 2 != 0) throw runtime_error("Ciphertext length must be even for Playfair decryption.");

    string out;
    out.reserve(s.size());

    for (size_t i = 0; i < s.size(); i += 2) {
        char A = s[i];
        char B = s[i + 1];
        int idxA = posIdx[A - 'A'];
        int idxB = posIdx[B - 'A'];
        if (idxA < 0 || idxB < 0) throw runtime_error("Invalid letter (after normalize) in decrypt.");

        int ra = idxA / 5, ca = idxA % 5;
        int rb = idxB / 5, cb = idxB % 5;

        if (ra == rb) {
            // same row -> shift left
            out.push_back(keySquare[ra][(ca + 5 - 1) % 5]);
            out.push_back(keySquare[rb][(cb + 5 - 1) % 5]);
        } else if (ca == cb) {
            // same column -> shift up
            out.push_back(keySquare[(ra + 5 - 1) % 5][ca]);
            out.push_back(keySquare[(rb + 5 - 1) % 5][cb]);
        } else {
            // rectangle -> swap columns
            out.push_back(keySquare[ra][cb]);
            out.push_back(keySquare[rb][ca]);
        }
    }
    return out;
}

// Create results.txt containing encryption and decryption for the lab vectors
void generate_results_file() {
    struct Test { string key; string plain; string expected_cipher; };
    vector<Test> tests = {
        {"MONARCHY", "INSTRUMENTS", "GATLMZCLRQXA"},
        {"PLAYFAIREXAMPLE", "HIDE THE GOLD IN THE TREE STUMP", "BMODZBXDNABEKUDMUIXMMOUVIF"},
        {"SECURITY", "BALLOON", "IBOQMPPO"},
        {"CRYPTOGRAPHY", "DEFEND THE EAST WALL OF THE CASTLE", "EFIFLFPBIVFGZBYFQUUDKYGIYOZBMD"},
        {"KNOWLEDGE", "KICK", "WCPE"}
    };

    ofstream fout("results.txt", ios::out | ios::trunc);
    if (!fout) throw runtime_error("Failed to open results.txt for writing.");

    for (size_t i = 0; i < tests.size(); ++i) {
        const auto &t = tests[i];
        buildKeySquare(t.key);
        string enc = encrypt(t.plain);
        string dec = decrypt(enc);

        fout << "# Test Vector " << (i + 1) << "\n";
        fout << "KEYWORD: " << t.key << "\n";
        fout << "PLAINTEXT: " << normalize(t.plain) << "\n";
        fout << "ENCRYPTED: " << enc << "\n";
        fout << "DECRYPTED: " << dec << "\n\n";
    }
    fout.close();
    cout << "results.txt written successfully (" << tests.size() << " vectors).\n";
}

int main(int argc, char **argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string mode, key, text;
    // Minimal flag parsing
    for (int i = 1; i < argc; ++i) {
        string a = argv[i];
        if (a == "--mode" && i + 1 < argc) mode = argv[++i];
        else if (a == "--key" && i + 1 < argc) key = argv[++i];
        else if (a == "--text" && i + 1 < argc) text = argv[++i];
    }

    // fallback to stdin three lines if no flags
    if (mode.empty()) {
        if (!getline(cin, mode)) return 0;
        getline(cin, key);
        getline(cin, text);
    }

    try {
        if (mode == "results") {
            generate_results_file();
            return 0;
        }
        if (mode == "enc") {
            if (key.empty() || text.empty()) {
                cerr << "Usage: --mode enc --key \"KEY\" --text \"TEXT\"\n";
                return 1;
            }
            buildKeySquare(key);
            cout << encrypt(text) << "\n";
            return 0;
        }
        if (mode == "dec") {
            if (key.empty() || text.empty()) {
                cerr << "Usage: --mode dec --key \"KEY\" --text \"TEXT\"\n";
                return 1;
            }
            buildKeySquare(key);
            cout << decrypt(text) << "\n";
            return 0;
        }

        cerr << "Invalid mode. Use --mode enc|dec|results\n";
        return 1;
    } catch (const exception &e) {
        cerr << "[ERROR] " << e.what() << "\n";
        return 2;
    }
}