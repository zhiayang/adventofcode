// prog.cpp
// Copyright (c) 2014 - 2016, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.


#if 0

#include <string>
#include <stdio.h>



// Caculate MD5 checksum
// Constants are the integer part of the sines of integers (in radians) * 2^32.
const uint32_t k[64] =
{
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

// r specifies the per-round shift amounts
const uint32_t r[] =
{
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

// leftrotate function definition
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

void to_bytes(uint32_t val, uint8_t* bytes)
{
	bytes[0] = (uint8_t) val;
	bytes[1] = (uint8_t) (val >> 8);
	bytes[2] = (uint8_t) (val >> 16);
	bytes[3] = (uint8_t) (val >> 24);
}

uint32_t to_int32(const uint8_t* bytes)
{
	return (uint32_t) bytes[0]
		| ((uint32_t) bytes[1] << 8)
		| ((uint32_t) bytes[2] << 16)
		| ((uint32_t) bytes[3] << 24);
}

uint8_t* domd5(uint8_t* initial_msg, uint64_t initial_len)
{

	// These vars will contain the hash
	uint32_t h0, h1, h2, h3;

	// Message (to prepare)
	uint8_t *msg = 0;

	uint64_t new_len, offset;
	uint32_t w[16];
	uint32_t a, b, c, d, i, f, g, temp;

	// Initialize variables - simple count in nibbles:
	h0 = 0x67452301;
	h1 = 0xefcdab89;
	h2 = 0x98badcfe;
	h3 = 0x10325476;

	//Pre-processing:
	//append "1" bit to message
	//append "0" bits until message length in bits ≡ 448 (mod 512)
	//append length mod (2^64) to message

	for (new_len = initial_len + 1; new_len % (512 / 8) != 448 / 8; new_len++);

	msg = new uint8_t[new_len + 8];
	memcpy(msg, initial_msg, initial_len);
	msg[initial_len] = 0x80; // append the "1" bit; most significant bit is "first"

	for(offset = initial_len + 1; offset < new_len; offset++)
		msg[offset] = 0; // append "0" bits

	// append the len in bits at the end of the buffer.
	to_bytes((uint32_t)(initial_len * 8), msg + new_len);

	// initial_len>>29 == initial_len*8>>32, but avoids overflow.
	to_bytes((uint32_t)(initial_len >> 29), msg + new_len + 4);

	// Process the message in successive 512-bit chunks:
	//for each 512-bit chunk of message:
	for(offset = 0; offset < new_len; offset += (512 / 8))
	{
		// break chunk into sixteen 32-bit words w[j], 0 ≤ j ≤ 15
		for(i = 0; i < 16; i++)
			w[i] = to_int32(msg + offset + i * 4);

		// Initialize hash value for this chunk:
		a = h0;
		b = h1;
		c = h2;
		d = h3;

		// Main loop:
		for(i = 0; i < 64; i++) {

			if (i < 16)
			{
				f = (b & c) | ((~b) & d);
				g = i;
			}
			else if (i < 32)
			{
				f = (d & b) | ((~d) & c);
				g = (5 * i + 1) % 16;
			}
			else if (i < 48)
			{
				f = b ^ c ^ d;
				g = (3 * i + 5) % 16;
			}
			else
			{
				f = c ^ (b | (~d));
				g = (7 * i) % 16;
			}

			temp = d;
			d = c;
			c = b;
			b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
			a = temp;

		}

		// Add this chunk's hash to result so far:
		h0 += a;
		h1 += b;
		h2 += c;
		h3 += d;

	}

	// cleanup
	delete msg;

	uint8_t* digest = new uint8_t[16];

	to_bytes(h0, digest);
	to_bytes(h1, digest + 4);
	to_bytes(h2, digest + 8);
	to_bytes(h3, digest + 12);

	return digest;
}


bool checkhash(uint8_t* hash)
{
	return (hash[0] == 0) && (hash[1] == 0) && ((hash[2] & 0xF0) == 0);
}


std::string lmao(char* c)
{
	std::string ret;
	for(int i = 0; i < 8; i++)
	{
		if(c[i] == 0) ret += "_";
		else ret += c[i];
	}

	return ret;
}

#define PART 2
int main()
{
	std::string input = "reyedfim";

	#if PART == 1
	{
		size_t k = 0;
		std::string password;

		while(password.length() < 8)
		{
			std::string sk = std::to_string(k);
			std::string s = input + sk;
			auto hash = domd5((uint8_t*) s.c_str(), s.length());

			fprintf(stderr, "\r                     \r(%s): %zu", password.c_str(), k);
			if(checkhash(hash))
			{
				password += char(hash[2] < 0xA ? (hash[2] + '0') : (hash[2] + 'a' - 0xA));
				// printf("%s\n", sk.c_str());
				printf("\n");
			}

			delete[] hash;

			k++;
		}

		printf("password = %s\n", password.c_str());
	}
	#else
	{
		char password[9] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };

		size_t k = 0;
		size_t filled = 0;
		while(filled < 8)
		{
			std::string sk = std::to_string(k);
			std::string s = input + sk;
			auto hash = domd5((uint8_t*) s.c_str(), s.length());

			fprintf(stderr, "\r                     \r(%s): %zu", lmao(password).c_str(), k);
			if(checkhash(hash))
			{
				char d = (hash[3] & 0xF0) >> 4;
				char pwc = (d < 0xA ? (d + '0') : (d + 'a' - 0xA));

				int pos = hash[2] & 0x0F;
				if(pos < 8 && password[pos] == 0)
				{
					password[pos] = pwc;
					printf("\n");
					filled++;
				}
			}

			delete[] hash;

			k++;
		}

		printf("password = %s\n", password);
	}
	#endif
}
#endif












