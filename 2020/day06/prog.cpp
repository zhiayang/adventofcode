// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

int main()
{
	// fuck you, topaz.
	auto fixed = replace(replace(replace(readFile("input.txt"), "\n\n", "|"), "\n", " "), "|", "\n");
	auto lines = splitString(fixed, '\n');

	int any = 0;
	int all = 0;
	for(auto& line : lines)
	{
		int yeses[26] = { 0 };
		auto anss = splitString(line, ' ');

		for(auto& ans : anss)
			for(char c : ans)
				yeses[c - 'a']++;

		for(int i : yeses)
		{
			if(i > 0)
				any++;

			if(i == anss.size())
				all++;
		}
	}

	zpr::println("part 1 = {}", any);
	zpr::println("part 2 = {}", all);
}









