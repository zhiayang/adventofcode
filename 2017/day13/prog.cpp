// prog.cpp
// Copyright (c) 2014 - 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <set>
#include <map>
#include <array>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

int main()
{
	std::vector<std::pair<int, int>> input = {
		{ 0, 4 },
		{ 1, 2 },
		{ 2, 3 },
		{ 4, 5 },
		{ 6, 6 },
		{ 8, 4 },
		{ 10, 8 },
		{ 12, 6 },
		{ 14, 6 },
		{ 16, 8 },
		{ 18, 8 },
		{ 20, 6 },
		{ 22, 8 },
		{ 24, 9 },
		{ 26, 8 },
		{ 28, 8 },
		{ 30, 12 },
		{ 32, 12 },
		{ 34, 10 },
		{ 36, 12 },
		{ 38, 12 },
		{ 40, 10 },
		{ 42, 12 },
		{ 44, 12 },
		{ 46, 12 },
		{ 48, 12 },
		{ 50, 12 },
		{ 52, 14 },
		{ 54, 14 },
		{ 56, 12 },
		{ 58, 14 },
		{ 60, 14 },
		{ 62, 14 },
		{ 64, 17 },
		{ 66, 14 },
		{ 70, 14 },
		{ 72, 14 },
		{ 74, 14 },
		{ 76, 14 },
		{ 78, 18 },
		{ 82, 14 },
		{ 88, 18 },
		{ 90, 14 }

		// { 0, 3 },
		// { 1, 2 },
		// { 4, 4 },
		// { 6, 4 },
	};

	// adjust
	int maxdepth = 90;


	std::map<int, int> ranges;
	for(auto [ d, r ] : input)
		ranges[d] = r;

	int pos = 0;
	int fails = 0;
	while(true)
	{
		int range = ranges[pos];
		if(range != 0)
		{
			int scanner = pos % ((range - 1) * 2);

			if(scanner == 0)
				fails += range * pos;
		}

		if(pos == maxdepth)
			break;

		pos++;
	}

	printf("fails = %d\n", fails);



	// brute-fckin-force it
	int delay = 0;
	while(true)
	{
		int pos = 0;
		bool fail = false;
		while(true)
		{
			int range = ranges[pos];
			if(range != 0)
			{
				int scanner = (pos + delay) % ((range - 1) * 2);

				if(scanner == 0)
				{
					fail = true;
					break;
				}
			}

			if(pos == maxdepth)
				break;

			pos++;
		}

		if(!fail)
			break;

		delay++;
		fprintf(stderr, "\r             \r%d", delay);
	}

	printf("\ndelay = %d\n", delay);
}








