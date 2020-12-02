// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"

int main()
{
	using namespace util;

	auto lines = readFileLines("input.txt", '\n');

	int part1 = 0;
	int part2 = 0;
	for(auto& line : lines)
	{
		auto xs = splitString(line, ' ');
		auto times = xs[0];
		auto ch = xs[1][0];
		auto pw = xs[2];

		auto ts = splitString(times, '-');
		auto t1 = to_int(ts[0]);
		auto t2 = to_int(ts[1]);

		auto count = std::count(pw.begin(), pw.end(), ch);
		if(t1 <= count && count <= t2)
			part1++;

		if((pw[t1 - 1] == ch) ^ (pw[t2 - 1] == ch))
			part2++;
	}

	zpr::println("part 2 = {}", part2);
	zpr::println("part 1 = {}", part1);
}









