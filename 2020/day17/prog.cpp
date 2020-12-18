// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

static int neighbours(std::map<v4, char>& map, v4 pos)
{
	int n = 0;
	for(int x = -1; x <= 1; x++)
		for(int y = -1; y <= 1; y++)
			for(int z = -1; z <= 1; z++)
				for(int w = -1; w <= 1; w++)
					if(x != 0 || y != 0 || z != 0 || w != 0)
						if(map[pos + v4(x, y, z, w)] == '#')
							n++;
	return n;
}

static void simulate(std::map<v4, char>& cubes, v4 min, v4 max)
{
	auto copy = cubes;
	for(auto x = min.x; x <= max.x; x++)
	{
		for(auto y = min.y; y <= max.y; y++)
		{
			for(auto z = min.z; z <= max.z; z++)
			{
				for(auto w = min.w; w <= max.w; w++)
				{
					auto pos = v4(x, y, z, w);

					auto c = cubes[pos];
					int n = neighbours(cubes, pos);

					if(c == '#')
					{
						if(n == 2 || n == 3) copy[pos] = '#';
						else                 copy[pos] = '.';
					}
					else
					{
						if(n == 3) copy[pos] = '#';
						else       copy[pos] = '.';
					}
				}
			}
		}
	}

	cubes = std::move(copy);
}

static int run(const std::map<v4, char>& start, int sx, int sy, bool four_d)
{
	auto cubes = start;

	auto min = v4(-1,     -1,     -1, four_d ? -1 : 0);
	auto max = v4(sx + 1, sy + 1, +1, four_d ? +1 : 0);

	for(int i = 0; i < 6; i++)
	{
		simulate(cubes, min, max);
		min -= v4(1, 1, 1, four_d ? 1 : 0);
		max += v4(1, 1, 1, four_d ? 1 : 0);
	}

	int sum = 0;
	for(auto& [ _, x ] : cubes)
	{
		if(x == '#')
			sum++;
	}

	return sum;
}

int main()
{
	std::map<v4, char> start;
	auto lines = readFileLines("input.txt");

	for(size_t y = 0; y < lines.size(); y++)
	{
		for(size_t x = 0; x < lines[y].size(); x++)
		{
			auto pos = v4(x, y, 0, 0);
			start[pos] = lines[y][x];
		}
	}

	auto sx = lines[0].size();
	auto sy = lines.size();

	zpr::println("part 1 = {}", run(start, sx, sy, false));
	zpr::println("part 2 = {}", run(start, sx, sy, true));
}
