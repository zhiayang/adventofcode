// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"

int main()
{
	auto lines = util::map(util::readFileLines("input.txt", '\n'), util::to_int);
	for(size_t i = 0; i < lines.size(); i++)
	{
		for(size_t j = 0; j < lines.size(); j++)
		{
			if(i == j)
				continue;

			if(lines[i] + lines[j] == 2020)
			{
				zpr::println("part 1 = {}", lines[i] * lines[j]);
				goto done;
			}
		}
	}

done:
	for(size_t i = 0; i < lines.size(); i++)
	{
		for(size_t j = 0; j < lines.size(); j++)
		{
			for(size_t k = 0; k < lines.size(); k++)
			{
				if(i != j && i != k && j != k)
				{
					if(lines[i] + lines[j] + lines[k] == 2020)
					{
						zpr::println("part 2 = {}", lines[i] * lines[j] * lines[k]);
						goto done2;
					}
				}
			}
		}
	}

done2:
	;
}
