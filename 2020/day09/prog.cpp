// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

int main()
{
	auto lines = map(readFileLines("input.txt"), to_int);

	size_t start = 0;
	size_t end = 25;

	int64_t target = 0;
	for(size_t i = 25; i < lines.size(); i++)
	{
		bool found = false;
		for(size_t x = start; !found && x < end; x++)
			for(size_t y = start; !found && y < end; y++)
				if(x != y && lines[x] != lines[y] && lines[x] + lines[y] == lines[i])
					found = true;

		if(!found)
		{
			zpr::println("part 1 = {}", lines[i]);
			target = lines[i];
			break;
		}

		start++;
		end++;
	}

	auto sum = [&lines](size_t begin, size_t end) -> int64_t {
		int64_t x = 0;
		for(size_t i = begin; i <= end; i++)
			x += lines[i];

		return x;
	};

	bool found = false;
	for(size_t i = 0; !found && i < lines.size(); i++)
	{
		for(size_t k = i; !found && k < lines.size(); k++)
		{
			auto s = sum(i, k);
			if(s == target)
			{
				zpr::println("part 2 = {}", lines[i] + lines[k]);
				found = true;
				break;
			}
			else if(s > target)
			{
				break;
			}
		}
	}
}









