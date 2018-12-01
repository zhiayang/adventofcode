// prog.cpp
// Copyright (c) 2014 - 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <set>
#include <map>
#include <array>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include <algorithm>

int main()
{
	const size_t _gA = 703;
	const size_t _gB = 516;
	const size_t fA = 16807;
	const size_t fB = 48271;
	const size_t magic = 2147483647;

	size_t gA = _gA;
	size_t gB = _gB;

	auto next = [](size_t factor, size_t prev) -> size_t {
		return (prev * factor) % magic;
	};

	int matches = 0;

	for(int i = 0; i < 40000000; i++)
	{
		if((gA & 0xFFFF) == (gB & 0xFFFF))
			matches++;

		gA = next(fA, gA);
		gB = next(fB, gB);
	}

	printf("matches = %d\n", matches);


	// next one

	auto next2 = [](size_t factor, size_t prev, size_t mod) -> size_t {

		size_t ret = 0;
		do {
			ret = (prev * factor) % magic;
			prev = ret;

		} while(ret % mod != 0);

		return ret;
	};


	gA = _gA;
	gB = _gB;
	matches = 0;

	for(int i = 0; i < 5000000; i++)
	{
		if((gA & 0xFFFF) == (gB & 0xFFFF))
			matches++;

		gA = next2(fA, gA, 4);
		gB = next2(fB, gB, 8);

		fprintf(stderr, "\r            \r%d", i);
	}

	printf("\nmatches = %d\n", matches);

}















