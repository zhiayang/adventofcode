// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

int main()
{
	auto lines = readFileLines("input.txt", '\n');

	std::unordered_map<int, bool> seen;

	int max = 0;
	for(auto& line : lines)
	{
		int row = 0;
		int col = 0;
		for(size_t i = 0; i < 7; i++)
		{
			if(line[i] == 'B')
				row |= (1 << (6 - i));
		}

		for(size_t i = 0; i < 3; i++)
		{
			if(line[7 + i] == 'R')
				col |= (1 << (2 - i));
		}

		int id = 8 * row + col;
		seen[id] = true;

		if(id > max)
			max = id;
	}

	zpr::println("part 1 = {}", max);

	for(int row = 0; row < 128; row++)
	{
		for(int col = 0; col < 8; col++)
		{
			int id = 8 * row + col;
			if(seen[id - 1] && seen[id + 1] && !seen[id])
				zpr::println("part 2 = {}", id);
		}
	}
}









