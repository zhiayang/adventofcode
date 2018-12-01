// prog.cpp
// Copyright (c) 2014 - 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <set>
#include <map>
#include <array>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include <algorithm>

std::string knothash(std::string input)
{
	std::vector<int> lengths;
	for(auto c : input)
		lengths.push_back((int) c);

	lengths.insert(lengths.end(), { 17, 31, 73, 47, 23 });

	std::vector<int> list;
	for(int i = 0; i < 256; i++)
		list.push_back(i);

	int skip = 0;
	int index = 0;
	for(int rnd = 0; rnd < 64; rnd++)
	{
		// std::vector<int> lengths = { 3, 4, 1, 5 };

		for(auto length : lengths)
		{
			// extract the selection
			auto first = list.begin() + index;

			std::vector<int>::iterator last;
			if(index + length > list.size())
				last = list.end();

			else
				last = first + length;

			auto sub = std::vector<int>(first, last);

			// then insert the remaining if any
			if(last - first < length)
				sub.insert(sub.end(), list.begin(), list.begin() + (length - (last - first)));

			std::reverse(sub.begin(), sub.end());

			// insert into the end
			auto it = first;
			auto i = 0;

			while(i < sub.size())
			{
				*it = sub[i];
				it++;
				i++;

				if(it == list.end())
					it = list.begin();
			}

			// for(auto l : list)
			// 	printf("%d ", l);

			// printf("\n");

			index += (length + skip);
			index %= list.size();
			skip++;
		}
	}

	// do the hash
	std::string hash;
	for(int i = 0; i < 256; i += 16)
	{
		uint8_t x = 0;
		for(int k = 0; k < 16; k++)
		{
			x ^= list[i + k];
		}

		hash += std::bitset<8>(x).to_string();
	}

	return hash;

}


int main()
{
	// std::string input = "flqrgnkx";
	std::string input = "jzgqcdpd";

	int set = 0;
	std::set<std::pair<int, int>> setbits;
	for(int y = 0; y < 128; y++)
	{
		auto bs = std::bitset<128>(knothash(input + "-" + std::to_string(y)));
		set += bs.count();

		for(int x = 0; x < 128; x++)
			if(bs[x]) setbits.insert({ x, y });
	}

	printf("set = %d\n", set);

	std::set<std::pair<int, int>> unseen = setbits;

	std::function<int (int, int)> fill = [&unseen, &fill, &setbits](int x, int y) -> int {
		if(unseen.find({ x, y }) == unseen.end())
			return 0;

		unseen.erase({ x, y });

		if(setbits.find({ x, y }) == setbits.end())
			return 0;

		return 1 + fill(x, y + 1) + fill(x + 1, y) + fill(x, y - 1) + fill(x - 1, y);
	};

	int regions = 0;
	while(unseen.size() > 0)
	{
		fill(unseen.begin()->first, unseen.begin()->second);
		regions++;
	}

	printf("regions = %d\n", regions);
}


















