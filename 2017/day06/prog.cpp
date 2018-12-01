// prog.cpp
// Copyright (c) 2014 - 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include <stdio.h>
#include <unistd.h>

#include <set>
#include <array>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

int main()
{
	auto getmax = [](const std::vector<int>& arr) -> int {
		int max = -1;
		int ret = 0;
		for(int i = 0; i < arr.size(); i++)
		{
			if(arr[i] > max)
				ret = i, max = arr[i];
		}

		return ret;
	};

	std::vector<int> input = { 4, 1, 15, 12, 0, 9, 9, 5, 5, 8, 7, 3, 14, 5, 12, 3 };
	std::vector<std::vector<int>> seen;

	seen.push_back(input);


	int steps = 0;
	int loopsz = 0;
	std::vector<int> loop;

	while(true)
	{
		auto k = getmax(input);
		auto num = input[k];

		input[k] = 0;

		int i = k;
		while(true)
		{
			if(num == 0)
				break;

			if(i == k) { i++; continue; }
			else if(i == input.size()) { i = 0; continue; }

			input[i]++;
			num--;

			i++;
		}

		steps++;
		if(loop.size() > 0)
			loopsz++;

		#if 0
		if(std::find(seen.begin(), seen.end(), input) != seen.end())
			break;
		#else
		if(loop.empty())
		{
			if(std::find(seen.begin(), seen.end(), input) != seen.end())
				loop = input;
		}
		else if(loop == input)
		{
			break;
		}

		#endif

		seen.push_back(input);
	}

	printf("steps = %d, loopsz = %d\n", steps, loopsz);
}





