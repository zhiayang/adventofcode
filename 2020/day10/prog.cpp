// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

static int64_t dfs(std::unordered_map<int, int64_t>& seen, std::unordered_map<int, bool>& adapters, int x, int target)
{
	if(!adapters[x]) return 0;
	if(x == target)  return 1;

	if(auto it = seen.find(x); it != seen.end())
		return it->second;

	int64_t ret = 0;
	for(int i = 1; i <= 3; i++)
		ret += dfs(seen, adapters, x + i, target);

	return (seen[x] = ret);
}


int main()
{
	auto lines = map(readFileLines("input.txt"), to_int);

	int target = 0;
	std::unordered_map<int, bool> adapters;
	{
		for(auto& line : lines)
			adapters[line] = true;

		target = 3 + maximum(lines);

		adapters[0] = true;
		adapters[target] = true;
	}

	int diff1 = 0;
	int diff3 = 0;

	int current = 0;
	while(true)
	{
	top:
		for(int i = 1; i <= 3; i++)
		{
			if(adapters[current + i])
			{
				if(i == 3) diff3++;
				if(i == 1) diff1++;

				current += i;
				goto top;
			}
		}

		break;
	}

	std::unordered_map<int, int64_t> seen;
	zpr::println("part 1 = {}", diff1 * diff3);
	zpr::println("part 2 = {}", dfs(seen, adapters, 0, target));
}









