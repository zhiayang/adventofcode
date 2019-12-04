// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"

static int run(std::map<int, int> x, int a, int b)
{
	x[1] = a;
	x[2] = b;

	size_t i = 0;
	while(x[i] != 99)
	{
		size_t len = 0;
		switch(x[i])
		{
			case 1: {
				len = 4;
				x[x[i + 3]] = x[x[i + 1]] + x[x[i + 2]];
			} break;

			case 2: {
				len = 4;
				x[x[i + 3]] = x[x[i + 1]] * x[x[i + 2]];
			} break;
		}

		i += len;
	}

	return x[0];
}

int main()
{
	auto ops = util::map(util::readFileLines("input.txt", ','), [](std::string_view s) -> int {
		return std::stoi(std::string(s));
	});

	std::map<int, int> prog;
	util::foreachIdx(ops, [&prog](int s, size_t i) {
		prog[i] = s;
	});

	{
		int res = run(prog, 12, 2);
		zpr::println("part 1: %d", res);
	}


	{
		for(const auto& [ noun, verb] : util::cartesian(util::rangeClosed(0, 99), util::rangeClosed(0, 99)))
		{
			if(run(prog, noun, verb) == 19690720)
			{
				zpr::println("part 2: %d", 100 * noun + verb);
				break;
			}
		}
	}
}









