// prog.cpp
// Copyright (c) 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include "assert.h"

#include <set>
#include <array>
#include <string>
#include <vector>
#include <unordered_set>

#include "aoc.h"
#include "tinyformat.h"

bool reacts(char a, char b)
{
	return (tolower(a) == tolower(b)) && (a != b);
}

std::string collapse(std::string poly)
{
	std::string prev;
	while(poly != prev)
	{
		prev = poly;
		for(size_t i = 0; i < poly.size(); i++)
		{
			if(i < poly.size() - 1 && reacts(poly[i], poly[i + 1]))
				poly.erase(i, 2);
		}
	}

	return poly;
}

int main()
{
	auto input = util::readFile("input.txt");

	tfm::printfln("part 1: len = %d", collapse(input).length());





	// try all the things.
	char best_char = 0;
	int best_len = 100000000;
	for(char unit = 'A'; unit <= 'Z'; unit++)
	{
		auto copy = input;
		copy.erase(std::remove(copy.begin(), copy.end(), unit), copy.end());
		copy.erase(std::remove(copy.begin(), copy.end(), tolower(unit)), copy.end());

		auto c = collapse(copy).length();
		if(c < best_len)
			best_len = c, best_char = unit;
	}

	tfm::printfln("part 2: best = %c / %d", best_char, best_len);
}
































