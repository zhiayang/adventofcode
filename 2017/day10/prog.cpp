// prog.cpp
// Copyright (c) 2014 - 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <map>
#include <array>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

template <typename I>
std::string n2hexstr(I w, size_t hex_len = sizeof(I) << 1)
{
	static const char* digits = "0123456789ABCDEF";
	std::string rc(hex_len, '0');

	for(size_t i = 0, j = (hex_len - 1) * 4 ; i < hex_len; ++i, j -= 4)
		rc[i] = digits[(w >> j) & 0x0F];

	return rc;
}


int main()
{
	// 1, 2), 3, 4, (5, 6, 7
	// 2, 1), 3, 4, (7, 6, 5
	// 5, 6, 7, 1, 2 -> 2, 1, 7, 6, 5
	// 6, 5), 3, 4, (2, 1, 7

	#if 0

	std::vector<int> lengths = { 94, 84, 0, 79, 2, 27, 81, 1, 123, 93, 218, 23, 103, 255, 254, 243 };
	// std::vector<int> lengths = { 3, 4, 1, 5 };

	std::vector<int> list;
	for(int i = 0; i < 256; i++)
		list.push_back(i);


	int skip = 0;
	int index = 0;
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

		for(auto l : list)
			printf("%d ", l);

		printf("\n");

		index += (length + skip);
		index %= list.size();
		skip++;
	}

	printf("%d x %d = %d\n", list[0], list[1], list[0] * list[1]);

	#else

	int skip = 0;
	int index = 0;

	std::string input = "94,84,0,79,2,27,81,1,123,93,218,23,103,255,254,243";
	// std::string input = "";

	std::vector<int> lengths;
	for(auto c : input)
		lengths.push_back((int) c);

	lengths.insert(lengths.end(), { 17, 31, 73, 47, 23 });

	std::vector<int> list;
	for(int i = 0; i < 256; i++)
		list.push_back(i);

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
		int x = 0;
		for(int k = 0; k < 16; k++)
		{
			x ^= list[i + k];
		}

		hash += n2hexstr(x, 2);
	}

	printf("hash = %s\n", hash.c_str());

	#endif
}











