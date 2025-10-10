## This project implements the Hill Cipher in C++.
It supports:
Reading plaintext from input.txt
Encrypting using a key matrix (mod 26)
Decrypting ciphertext with the matrix inverse modulo 26
Writing results to cipher.txt and decrypted.txt
Auto-creating input/output files if missing

## Implementation Details in Code

1) Determinant

int det = (K[0][0]*K[1][1] - K[0][1]*K[1][0]) % 26;
if (det < 0) det += 26;

2) Determinant inverse

int detInv = modInverse(det, 26);
Here, modInverse checks which number x satisfies (det*x)

3) Adjugate matrix

vector<vector<int>> Kinv(2, vector<int>(2));
Kinv[0][0] =  K[1][1] * detInv % 26;
Kinv[0][1] = -K[0][1] * detInv % 26;
Kinv[1][0] = -K[1][0] * detInv % 26;
Kinv[1][1] =  K[0][0] * detInv % 26;

4) Modulo correction

Since negative numbers may occur, we adjust them to be within [0,25]
if (Kinv[i][j] < 0) Kinv[i][j] += 26;

5) Usage in decryption

The inverse matrix is multiplied with ciphertext blocks to retrieve plaintext. 