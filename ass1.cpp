#include <iostream>
#include <string>
using namespace std;

char shiftChar(char c, int shift) {
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    if (c >= 'a' && c <= 'z') {
        c = c - 'a' + 'A'; 
    }

    for (int i = 0; i < 26; ++i) {
        if (c == alphabet[i]) {
            int shiftedIndex = (i + shift) % 26;
            return alphabet[shiftedIndex]; 
        }
    }

    return c; 
}

int main() {
    string input;
    int shift = 6;

    cout << "Enter a string: ";
    getline(cin, input);

    string result = "";
    for (char c : input) {
        result += shiftChar(c, shift);
    }

    cout << "Shifted string: " << result << endl;

    return 0;
}