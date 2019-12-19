// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"


static int pattern(int* xs, size_t len, int out_n)
{
	constexpr int coeffs[] = { 0, 1, 0, -1 };

	int sum = 0;
	for(size_t i = 0; i < len; i++)
	{
		auto ofs = i + 1;
		auto idx = (ofs / (out_n + 1)) % 4;

		auto coeff = coeffs[idx];
		// printf("%d * %d\n", xs[i], coeff);
		sum += xs[i] * coeff;
	}

	// printf("\n");
	return abs(sum) % 10;
}

static int* fft(int* xs, size_t len)
{
	auto out = new int[len];
	for(size_t i = 0; i < len; i++)
		out[i] = pattern(xs, len, i);

	return out;
}

int main()
{
	auto s = util::readFileRaw("input.txt");


	{
		auto length = s.size();
		auto input = new int[length];

		for(size_t i = 0; i < length; i++)
			input[i] = s[i] - '0';

		for(int i = 0; i < 100; i++)
		{
			auto input1 = fft(input, length);
			delete[] input;

			input = input1;
		}

		zpr::print("part 1: ");
		for(size_t i = 0; i < 8; i++)
			zpr::print("%d", input[i]);

		zpr::print("\n");
	}


	{
		auto length = s.size() * 10000;
		auto input = new int[length];

		for(size_t i = 0; i < length; i++)
			input[i] = s[i % s.size()] - '0';

		size_t offset = input[6] * 1
						+ input[5] * 10
						+ input[4] * 100
						+ input[3] * 1000
						+ input[2] * 10000
						+ input[1] * 100000
						+ input[0] * 1000000;

		for(int i = 0; i < 100; i++)
		{
			auto sum = input[length - 1];
			for(size_t k = length - 1; k-- > offset; )
			{
				sum += input[k];
				input[k] = sum % 10;
			}
		}

		zpr::print("part 2: ");
		for(size_t i = 0; i < 8; i++)
			printf("%d", input[offset + i]);

		zpr::print("\n");
	}
}











