// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"

static int calc_fuel(int x)
{
	return (x / 3) - 2;
}

int main()
{
	auto mods = util::map(util::readFileLines("input.txt"), [](const std::string& s) -> int {
		return std::stol(s);
	});

	{
		auto sum = util::sum(util::map(mods, calc_fuel));
		zpr::println("part 1: %d", sum);
	}

	{
		auto sum = util::sum(util::flatmap(mods, [](int mass) -> auto {
			// start with the fuel needed for the mass. then, while the fuel is > 0,
			// get the amount of fuel needed for that fuel (using that as the mass).
			return util::iterateWhile(
				/* seed: */ calc_fuel(mass),
				/* pred: */ [](int fuel) -> bool { return fuel > 0; },
				/* iter: */ calc_fuel
			);
		}));

		zpr::println("part 2: %d", sum);
	}
}
