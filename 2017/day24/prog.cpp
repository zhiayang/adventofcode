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

int main()
{
	using pair = std::pair<int, int>;
	std::vector<pair> input = {
		{ 32, 31 },
		{ 2, 2 },
		{ 0, 43 },
		{ 45, 15 },
		{ 33, 24 },
		{ 20, 20 },
		{ 14, 42 },
		{ 2, 35 },
		{ 50, 27 },
		{ 2, 17 },
		{ 5, 45 },
		{ 3, 14 },
		{ 26, 1 },
		{ 33, 38 },
		{ 29, 6 },
		{ 50, 32 },
		{ 9, 48 },
		{ 36, 34 },
		{ 33, 50 },
		{ 37, 35 },
		{ 12, 12 },
		{ 26, 13 },
		{ 19, 4 },
		{ 5, 5 },
		{ 14, 46 },
		{ 17, 29 },
		{ 45, 43 },
		{ 5, 0 },
		{ 18, 18 },
		{ 41, 22 },
		{ 50, 3 },
		{ 4, 4 },
		{ 17, 1 },
		{ 40, 7 },
		{ 19, 0 },
		{ 33, 7 },
		{ 22, 48 },
		{ 9, 14 },
		{ 50, 43 },
		{ 26, 29 },
		{ 19, 33 },
		{ 46, 31 },
		{ 3, 16 },
		{ 29, 46 },
		{ 16, 0 },
		{ 34, 17 },
		{ 31, 7 },
		{ 5, 27 },
		{ 7, 4 },
		{ 49, 49 },
		{ 14, 21 },
		{ 50, 9 },
		{ 14, 44 },
		{ 29, 29 },
		{ 13, 38 },
		{ 31, 11 }

		// { 0, 2 },
		// { 2, 2 },
		// { 2, 3 },
		// { 3, 4 },
		// { 3, 5 },
		// { 0, 1 },
		// { 10, 1 },
		// { 9, 10 }
	};

	std::vector<pair> starts;

	for(auto it = input.begin(); it != input.end();)
	{
		bool found = false;
		if(it->first == 0)
			starts.push_back(*it), found = true;

		else if(it->second == 0)
			starts.push_back({ it->second, it->first }), found = true;


		if(found)
			it = input.erase(it);

		else
			it++;
	}


	// note: the nodes in 'bridge' are re-ordered so their 'ports' line up
	// makes our lives easier

	// auto print = [](const std::vector<pair>& bridge) {
	// 	for(auto p : bridge)
	// 		printf("(%d, %d) - ", p.first, p.second);

	// 	printf("\n");
	// };


	std::function<void (const std::vector<pair>& bridge, std::vector<pair> pairs,
		std::vector<std::vector<pair>>& bridges, int)> recurse;

	recurse = [&recurse](const std::vector<pair>& bridge, std::vector<pair> pairs, std::vector<std::vector<pair>>& bridges,
		int nest) {

		// printf("begin: %d (%ld)\n", nest, pairs.size());

		bridges.push_back(bridge);
		for(size_t i = 0; i < pairs.size(); i++)
		{
			if(pairs[i].first == bridge.back().second)
			{
				auto copy = bridge;
				copy.push_back(pairs[i]);

				auto pairscopy = pairs;
				pairscopy.erase(pairscopy.begin() + i);

				recurse(copy, pairscopy, bridges, nest + 1);
			}
			else if(pairs[i].second == bridge.back().second)
			{
				auto copy = bridge;
				copy.push_back({ pairs[i].second, pairs[i].first });	// add, but flip it.

				auto pairscopy = pairs;
				pairscopy.erase(pairscopy.begin() + i);

				recurse(copy, pairscopy, bridges, nest + 1);
			}

		}
		// printf("end: %d\n\n", nest);
	};

	auto getstrength = [](const std::vector<pair>& bridge) -> int {
		int ret = 0;
		for(auto p : bridge)
			ret += (p.first + p.second);

		return ret;
	};

	int maxstr = 0;

	int maxlength = 0;
	int maxlongstrength = 0;
	for(auto start : starts)
	{
		std::vector<std::vector<pair>> bridges;
		recurse({ start }, input, bridges, 0);

		for(auto bridge : bridges)
		{
			auto str = getstrength(bridge);
			if(bridge.size() >= maxlength)
				maxlength = bridge.size(), maxlongstrength = std::max(str, maxlongstrength);

			// printf("%d: ", str);
			// print(bridge);

			maxstr = std::max(str, maxstr);
		}
	}

	printf("max strength = %d\n", maxstr);
	printf("max length of longest (%d) = %d\n", maxlength, maxlongstrength);
}
















