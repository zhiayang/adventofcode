// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

static inline int& get(std::vector<int>& seen, int n)
{
	if(n >= seen.size())
		seen.resize(2 * (n + 1));

	return seen[n];
}

static inline int update(std::vector<int>& seen, int turn, int last)
{
	int speak = 0;
	auto& a = get(seen, last);
	if(a == 0)  speak = 0;
	else        speak = turn - a - 1;

	a = turn - 1;
	return speak;
}

int main()
{
	auto file = readFile("input.txt");
	auto input = map(split(file, ","), to_int);

	int turn = 1;
	std::vector<int> seen;

	for(auto n : input)
		get(seen, n) = turn++;

	int last = input.back();
	while(true)
	{
		last = update(seen, turn, last);

		if(turn == 2020)
		{
			zpr::println("part 1 = {}", last);
		}
		else if(turn == 30000000)
		{
			zpr::println("part 2 = {}", last);
			break;
		}

		turn++;
	}
}

