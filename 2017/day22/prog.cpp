// prog.cpp
// Copyright (c) 2014 - 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <set>
#include <map>
#include <deque>
#include <array>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include <algorithm>
#include <string_view>
#include <unordered_map>

int main()
{
	std::vector<std::string> input = {
		"#.##.###.#.#.##.###.##.##",
		".##.#.#.#..####.###.....#",
		"...##.....#..###.#..#.##.",
		"##.###.#...###.#.##..##.#",
		"###.#.###..#.#.##.#.###.#",
		".###..#.#.####..##..#..##",
		"..###.##..###.#..#...###.",
		"........##..##..###......",
		"######...###...###...#...",
		".######.##.###.#.#...###.",
		"###.##.###..##..#..##.##.",
		".#.....#.#.#.#.##........",
		"#..#..#.#...##......#.###",
		"#######.#...#..###..#..##",
		"#..#.###...#.#.#.#.#....#",
		"#.#####...#.##.##..###.##",
		"..#..#..#.....#...#.#...#",
		"###.###.#...###.#.##.####",
		".....###.#..##.##.#.###.#",
		"#..#...######.....##.##.#",
		"###.#.#.#.#.###.##..###.#",
		"..####.###.##.#.###..#.##",
		"#.#....###....##...#.##.#",
		"###..##.##.#.#.##..##...#",
		"#.####.###.#...#.#.....##"

		// "..#",
		// "#..",
		// "..."
	};


	#if 0
	// i don't actually think the coordinates matter.
	// just that we start in the middle.
	std::map<std::pair<int, int>, bool> map;
	for(int y = 0; y < input.size(); y++)
	{
		for(int x = 0; x < input[y].size(); x++)
			map[{ x, y }] = (input[y][x] == '#');
	}

	int x = 12;
	int y = 12;

	int dir = 0;	// 0 = up, 1 = right, 2 = down, 3 = left
	int count = 0;
	for(int step = 0; step < 10000; step++)
	{
		bool infected = map[{ x, y }];
		if(infected)	dir++;
		else			dir--;

		if(dir == -1) dir = 3;

		dir %= 4;
		dir = std::abs(dir);

		// printf("(%d, %d) = %d / %d\n", x, y, infected, dir);

		map[{ x, y }] = !infected;

		if(!infected)
			count++;

		if(dir == 0)		y--;
		else if(dir == 1)	x++;
		else if(dir == 2)	y++;
		else if(dir == 3)	x--;
	}

	printf("count = %d\n", count);
	#else


	// state:
	// 0 = clean
	// 1 = weak
	// 2 = infected
	// 3 = flagged
	std::map<std::pair<int, int>, int> map;
	for(int y = 0; y < input.size(); y++)
	{
		for(int x = 0; x < input[y].size(); x++)
			map[{ x, y }] = (input[y][x] == '#' ? 2 : 0);
	}

	int x = 12;
	int y = 12;

	int dir = 0;	// 0 = up, 1 = right, 2 = down, 3 = left
	int count = 0;
	for(int step = 0; step < 10000000; step++)
	{
		int state = map[{ x, y }];
		if(state == 0)		dir--;
		else if(state == 1)	;
		else if(state == 2)	dir++;
		else if(state == 3)	dir += 2;

		if(dir == -1) dir = 3;

		dir %= 4;
		dir = std::abs(dir);

		// printf("(%d, %d) = %d / %d\n", x, y, infected, dir);

		map[{ x, y }] = (state + 1) % 4;

		// weakened nodes become infected
		if(state == 1) count++;

		if(dir == 0)		y--;
		else if(dir == 1)	x++;
		else if(dir == 2)	y++;
		else if(dir == 3)	x--;
	}

	printf("count = %d\n", count);

	#endif
}












