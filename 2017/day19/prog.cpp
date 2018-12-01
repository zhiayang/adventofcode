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
#include <fstream>
#include <algorithm>
#include <string_view>

int main()
{
	std::vector<std::string> input;

	auto in = std::ifstream("day19/input.txt");
	std::string str;
	while(std::getline(in, str))
		input.push_back(str);


	// find the entrance.
	int x = 0;
	int y = 0;

	for(x = 0; x < input[y].size(); x++)
	{
		if(input[y][x] == '|')
			break;
	}

	// ok.
	int dx = 0;
	int dy = 1;
	int px = 0;
	int py = 0;

	int steps = 0;
	std::string path;
	while(true)
	{
		if(input[y][x] == '+')
		{
			// search around.
			if(input[y - 1][x] == '|' && y -1 != py)
			{
				dx = 0;
				dy = -1;
				printf("up\n");
			}
			else if(input[y + 1][x] == '|' && y + 1 != py)
			{
				dx = 0;
				dy = 1;
				printf("down\n");
			}
			else if(input[y][x - 1] == '-' && x - 1 != px)
			{
				dx = -1;
				dy = 0;
				printf("left\n");
			}
			else if(input[y][x + 1] == '-' && x + 1 != px)
			{
				dx = 1;
				dy = 0;
				printf("right\n");
			}
			else
			{
				// uh.
				printf("stuck\n");
				break;
			}
		}
		else if(isalpha(input[y][x]))
		{
			path += input[y][x];
		}

		px = x;
		py = y;

		x += dx;
		y += dy;

		steps++;

		printf("(%d, %d)\n", x, y);

		// check if we haven't died
		if(isspace(input[y][x]))
			break;
	}

	printf("path = %s, %d steps\n", path.c_str(), steps);
}














