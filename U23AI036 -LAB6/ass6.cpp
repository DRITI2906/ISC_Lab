#include <bits/stdc++.h>
using namespace std;

// Modular inverse using Extended Euclidean Algorithm
int modInverse(int a, int m) {
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1; // no inverse
}

int main() {
    int mod = 26;
    vector<vector<int>> K = {{3,3},{2,5}}; // key matrix (2x2)

    // --- Step 1: Read input ---
    ifstream fin("input.txt");
    if (!fin.is_open()) {
        ofstream create("input.txt");
        create << "HELP"; // default
        create.close();
        cout << "Created 'input.txt'. Please put your text and run again.\n";
        return 0;
    }
    string text;
    getline(fin, text);
    fin.close();

    // Preprocess: keep only A-Z, uppercase
    string plain;
    for (char c : text) if (isalpha(c)) plain += toupper(c);

    // Pad if needed
    while (plain.size() % 2 != 0) plain += 'X';

    // Convert to numbers
    vector<int> nums;
    for (char c : plain) nums.push_back(c - 'A');

    // --- Step 2: Encrypt ---
    vector<int> cipher_nums;
    for (int i = 0; i < nums.size(); i += 2) {
        int x = (nums[i] * K[0][0] + nums[i+1] * K[1][0]) % mod;
        int y = (nums[i] * K[0][1] + nums[i+1] * K[1][1]) % mod;
        cipher_nums.push_back(x);
        cipher_nums.push_back(y);
    }
    string cipher;
    for (int n : cipher_nums) cipher.push_back(n + 'A');

    ofstream fout1("cipher.txt");
    fout1 << cipher;
    fout1.close();

    // --- Step 3: Decrypt ---
    int det = (K[0][0]*K[1][1] - K[0][1]*K[1][0]) % mod;
    if (det < 0) det += mod;
    int detInv = modInverse(det, mod);

    // Inverse matrix
    vector<vector<int>> Kinv(2, vector<int>(2));
    Kinv[0][0] =  K[1][1] * detInv % mod;
    Kinv[0][1] = -K[0][1] * detInv % mod;
    Kinv[1][0] = -K[1][0] * detInv % mod;
    Kinv[1][1] =  K[0][0] * detInv % mod;

    for (int i=0;i<2;i++) for (int j=0;j<2;j++) {
        if (Kinv[i][j] < 0) Kinv[i][j] += mod;
    }

    vector<int> dec_nums;
    for (int i = 0; i < cipher_nums.size(); i += 2) {
        int x = (cipher_nums[i] * Kinv[0][0] + cipher_nums[i+1] * Kinv[1][0]) % mod;
        int y = (cipher_nums[i] * Kinv[0][1] + cipher_nums[i+1] * Kinv[1][1]) % mod;
        dec_nums.push_back(x);
        dec_nums.push_back(y);
    }
    string decrypted;
    for (int n : dec_nums) decrypted.push_back(n + 'A');

    ofstream fout2("decrypted.txt");
    fout2 << decrypted;
    fout2.close();

    cout << "✅ Done. Check cipher.txt and decrypted.txt\n";
    return 0;
}