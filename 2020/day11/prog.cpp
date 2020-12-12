// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

static int solve(grid<char> seats, int thresh, int (*adj_fn)(const grid<char>&, v2))
{
	auto new_seats = seats;

	while(true)
	{
		seats.iter_xy([&](auto x, auto y) {
			auto pos = v2(x, y);
			auto adj = adj_fn(seats, pos);
			if(adj >= thresh && seats[pos] == '#')
				new_seats[pos] = 'L';
			else if(adj == 0 && seats[pos] == 'L')
				new_seats[pos] = '#';
			else
				new_seats[pos] = seats[pos];
		});

		if(new_seats == seats)
			break;

		seats = new_seats;
	}

	return seats.count('#');
}


int main()
{
	auto seats = grid<char>::from_lines(readFileLines("input.txt"));

	zpr::println("part 1 = {}", solve(seats, 4, [](const auto& seats, v2 pos) {
		v2 dirs[] = { v2(+1, +1), v2(+1, -1), v2(-1, +1), v2(-1, -1), v2(0, +1), v2(0, -1), v2(-1, 0), v2(+1, 0) };
		int ret = 0;
		for(auto& d : dirs)
		{
			auto v = pos + d;
			if(seats.in_bounds(v))
				ret += (seats[v] == '#');
		}

		return ret;
	}));

	zpr::println("part 2 = {}", solve(seats, 5, [](const auto& seats, v2 pos) {
		v2 dirs[] = { v2(+1, +1), v2(+1, -1), v2(-1, +1), v2(-1, -1), v2(0, +1), v2(0, -1), v2(-1, 0), v2(+1, 0) };

		int ret = 0;
		for(auto& d : dirs)
		{
			auto v = pos + d;
			while(seats.in_bounds(v))
			{
				if(seats[v] == '.') { v += d; continue; }
				else                { ret += (seats[v] == '#'); break; }
			}
		}

		return ret;
	}));
}









