// Minimal SHA-1 implementation (lab simplified version)
// Prints only the final 40-hex-digit digest for the input message.

#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::array;
using std::cin;
using std::cout;
using std::endl;
using std::hex;
using std::setfill;
using std::setw;
using std::string;
using std::uint32_t;
using std::uint64_t;
using std::uint8_t;
using std::vector;

static inline uint32_t rotl(uint32_t x, unsigned s)
{
	return (x << s) | (x >> (32U - s));
}

// Pad message per SHA-1: append 1 bit, zeros, then 64-bit big-endian length (in bits)
static vector<uint8_t> sha1_pad(const vector<uint8_t> &msg)
{
	vector<uint8_t> y = msg;
	// original length in bits stored in 64-bit big-endian
	uint64_t bit_len = static_cast<uint64_t>(msg.size()) * 8ULL;

	// append 0x80 (1 bit then 7 zeros)
	y.push_back(0x80);

	// pad with zeros until length mod 64 == 56
	while ((y.size() % 64) != 56)
	{
		y.push_back(0x00);
	}

	// append 64-bit big-endian length
	for (int i = 7; i >= 0; --i)
	{
		y.push_back(static_cast<uint8_t>((bit_len >> (i * 8)) & 0xFF));
	}
	return y;
}

// Format 5-word state as hex string (40 hex digits)
// (Helper removed: not needed in minimal output.)

// Compute SHA-1 digest, returning 20 bytes and filling optional per-block states
struct Sha1Result
{
	array<uint8_t, 20> digestBytes{};
	array<uint32_t, 5> Hfinal{};
};

static Sha1Result sha1_compute(const vector<uint8_t> &message)
{
	// Initial hash values
	uint32_t H0 = 0x67452301u;
	uint32_t H1 = 0xEFCDAB89u;
	uint32_t H2 = 0x98BADCFEu;
	uint32_t H3 = 0x10325476u;
	uint32_t H4 = 0xC3D2E1F0u;

	vector<uint8_t> y = sha1_pad(message);

	const size_t nBlocks = y.size() / 64;
	for (size_t block = 0; block < nBlocks; ++block)
	{
		// Prepare message schedule W[0..79]
		uint32_t W[80];
		// W0..W15 from the 512-bit block (big-endian)
		const uint8_t *p = &y[block * 64];
		for (int t = 0; t < 16; ++t)
		{
			W[t] = (static_cast<uint32_t>(p[4 * t]) << 24) |
				   (static_cast<uint32_t>(p[4 * t + 1]) << 16) |
				   (static_cast<uint32_t>(p[4 * t + 2]) << 8) |
				   (static_cast<uint32_t>(p[4 * t + 3]));
		}
		for (int t = 16; t < 80; ++t)
		{
			W[t] = rotl(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);
		}

		// Initialize working vars
		uint32_t A = H0;
		uint32_t B = H1;
		uint32_t C = H2;
		uint32_t D = H3;
		uint32_t E = H4;

		// Main loop
		for (int t = 0; t < 80; ++t)
		{
			uint32_t f, K;
			if (t <= 19)
			{
				f = (B & C) | ((~B) & D);
				K = 0x5A827999u;
			}
			else if (t <= 39)
			{
				f = B ^ C ^ D;
				K = 0x6ED9EBA1u;
			}
			else if (t <= 59)
			{
				f = (B & C) | (B & D) | (C & D);
				K = 0x8F1BBCDCu;
			}
			else
			{
				f = B ^ C ^ D;
				K = 0xCA62C1D6u;
			}
			uint32_t temp = rotl(A, 5) + f + E + W[t] + K;
			E = D;
			D = C;
			C = rotl(B, 30);
			B = A;
			A = temp;
		}

		// Add this chunk's hash to result so far
		H0 += A;
		H1 += B;
		H2 += C;
		H3 += D;
		H4 += E;

		// (Intermediate state printing removed in minimal version.)
	}

	Sha1Result res;
	res.Hfinal = {H0, H1, H2, H3, H4};
	// Convert to bytes big-endian
	uint32_t Hs[5] = {H0, H1, H2, H3, H4};
	for (int i = 0; i < 5; ++i)
	{
		res.digestBytes[i * 4 + 0] = static_cast<uint8_t>((Hs[i] >> 24) & 0xFF);
		res.digestBytes[i * 4 + 1] = static_cast<uint8_t>((Hs[i] >> 16) & 0xFF);
		res.digestBytes[i * 4 + 2] = static_cast<uint8_t>((Hs[i] >> 8) & 0xFF);
		res.digestBytes[i * 4 + 3] = static_cast<uint8_t>((Hs[i] >> 0) & 0xFF);
	}
	return res;
}

static string bytes_to_hex(const uint8_t *data, size_t len)
{
	std::ostringstream oss;
	oss << std::hex << std::nouppercase << std::setfill('0');
	for (size_t i = 0; i < len; ++i)
	{
		oss << std::setw(2) << static_cast<unsigned>(data[i]);
	}
	return oss.str();
}

int main(int argc, char **argv)
{
	// Get input message
	string input;
	if (argc > 1)
	{
		// Join args with spaces to form the message
		std::ostringstream oss;
		for (int i = 1; i < argc; ++i)
		{
			if (i > 1)
				oss << ' ';
			oss << argv[i];
		}
		input = oss.str();
	}
	else
	{
		cout << "Enter input message: ";
		std::getline(cin, input);
	}

	// Convert input string to bytes (UTF-8 as bytes of string)
	vector<uint8_t> msg(input.begin(), input.end());

	// Compute SHA-1 (minimal: no debug / padding display)
	Sha1Result res = sha1_compute(msg);
	string digestHex = bytes_to_hex(res.digestBytes.data(), res.digestBytes.size());
	cout << digestHex << "\n";

	return 0;
}
