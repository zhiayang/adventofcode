// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

static int64_t fast_exp(int64_t a, int64_t x, int64_t P)
{
	// this is basically exponentiation by squaring. 2^16 = 4^8 = 16^4 = 256^2
	if(x == 1) return a % P;

	if(x % 2 == 0)
	{
		return fast_exp((a * a) % P, x / 2, P) % P;
	}
	else
	{
		return a * (fast_exp((a * a) % P, (x - 1) / 2, P) % P) % P;
	}
}

static int64_t ext_gcd(int64_t a, int64_t b, int64_t* x, int64_t* y)
{
	if(b == 0)
	{
		*x = 1;
		*y = 0;
		return a;
	}

	int64_t x1 = 0;
	int64_t y1 = 0;

	auto gcd = ext_gcd(b, a % b, &x1, &y1);

	*x = y1;
	*y = x1 - (a / b) * y1;

	return gcd;
}

static int64_t modular_inverse(int64_t a, int64_t n)
{
	int64_t x = 0; int64_t y = 0;

	if(a < n) std::swap(a, n);
	ext_gcd(a, n, &x, &y);

	// y is the inverse.
	return y;
}

// compute base ^ power (mod M)
static int64_t modular_exp(int64_t base, int64_t power, int64_t mod)
{
	if(power == 0)
	{
		return 1;
	}
	else if(power < 0)
	{
		// c = b^e (mod m)  <=>  d^(−e) (mod m)
		// where e < 0 and b ⋅ d ≡ 1 (mod m)
		auto d = modular_inverse(base, mod);
		base = d;
		power = -power;
	}

	return fast_exp(base, power, mod);
}

// find X where seed^X = key (mod prime)
static int64_t break_rsa(int64_t seed, int64_t key, int64_t prime)
{
	auto m = (int64_t) std::ceil(std::sqrt((double) (prime - 1)));

	std::unordered_map<int64_t, int64_t> table;
	for(int64_t i = 0; i < m; i++)
		table[(key * modular_exp(seed, i, prime)) % prime] = i;

	for(int64_t j = 1; j <= m; j++)
	{
		auto x = modular_exp(seed, m * j, prime);
		if(auto it = table.find(x); it != table.end())
		{
			auto i = it->second;
			return (m * j) - i;
		}
	}

	assert(false && "no solution");
	return 0;
}

static int64_t transform(int64_t subject, int64_t loops, int64_t prime)
{
	int64_t result = 1;
	while(loops-- > 0)
	{
		result *= subject;
		result %= prime;
	}

	return result;
}

static int64_t break_rsa_3head(int64_t subject, int64_t key, int64_t prime)
{
	int64_t loops = 0;
	int64_t value = 1;
	while(true)
	{
		loops++;
		value *= subject;
		value %= prime;

		if(value == key)
			return loops;
	}
}

int main()
{
	static constexpr int64_t PRIME = 20201227;

	// COAL IN YOUR FUCKING SOCKS, ERIC WASTL
	auto input = readFileLines("input.txt");

	auto card = to_int(input[0]);
	auto door = to_int(input[1]);

	auto card_loops = break_rsa(7, card, PRIME);
	auto door_loops = break_rsa(7, door, PRIME);

	zpr::println("part 1 = {}", transform(card, door_loops, PRIME));
	zpr::println("part 2 = {}", "merry fucking christmas");
}
