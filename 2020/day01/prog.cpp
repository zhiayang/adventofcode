// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"

int main()
{
	auto lines = util::map(util::readFileLines("input.txt", '\n'), util::to_int);
	for(size_t i = 0; i < lines.size(); i++)
	{
		for(size_t j = i; j < lines.size(); j++)
		{
			if(lines[i] + lines[j] == 2020)
			{
				zpr::println("part 1 = {}", lines[i] * lines[j]);
			}
		}
	}

done:
	for(size_t i = 0; i < lines.size(); i++)
	{
		for(size_t j = i; j < lines.size(); j++)
		{
			for(size_t k = j; k < lines.size(); k++)
			{
				if(lines[i] + lines[j] + lines[k] == 2020)
				{
					zpr::println("part 2 = {}", lines[i] * lines[j] * lines[k]);
				}
			}
		}
	}

done2:
	;
}
