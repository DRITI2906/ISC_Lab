#!/bin/bash
set -e

echo "[*] Building..."
g++ -O2 -std=c++17 affine.cpp -o affine

echo "[*] Running tests..."

echo "Test 1: Encrypt HELLOWORLD (a=5, b=8)"
./affine --mode enc --a 5 --b 8 --in HELLOWORLD

echo "Test 2: Decrypt RCLLAOAPLX (a=5, b=8)"
./affine --mode dec --a 5 --b 8 --in RCLLAOAPLX

echo "Test 3: Crack ciphertext RCLLAOAPLX"
./affine --mode crack --in RCLLAOAPLX

echo "Test 4: Encrypt CRYPTOGRAPHY (a=7, b=3)"
./affine --mode enc --a 7 --b 3 --in CRYPTOGRAPHY

echo "Test 5: Decrypt RSPEGXTSDEAP (a=7, b=3)"
./affine --mode dec --a 7 --b 3 --in RSPEGXTSDEAP
