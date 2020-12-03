// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"

int main()
{
	using namespace util;

	auto lines = readFileLines("input.txt", '\n');

	auto check = [&lines](size_t dx, size_t dy) -> size_t {
		size_t trees = 0;

		size_t x = 0;
		size_t y = 0;

		while(y < lines.size())
		{
			auto& line = lines[y];
			x = x % line.size();

			if(line[x] == '#')
				trees++;

			x += dx;
			y += dy;
		}

		return trees;
	};

	zpr::println("part 1 = {}", check(3, 1));
	zpr::println("part 2 = {}", check(1, 1) * check(3, 1) * check(5, 1) * check(7, 1) * check(1, 2));
}









