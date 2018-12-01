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

#include "lexer.cpp"

int main()
{
	std::string progs = "abcdefghijklmnop";
	// for(int i = 0; i < 16; i++)
	// 	progs.push_back('a' + i);


	int steps = 1e9;

	auto input = readEntireFile("day16/input.txt");


	std::vector<std::string> seen;

	for(int step = 0; step < steps; step++)
	{
		if(auto it = std::find(seen.begin(), seen.end(), progs); it != seen.end())
		{
			int cycle = (seen.begin() + step) - it;
			while(step + cycle < steps)
				step += cycle;
		}
		else
		{
			seen.push_back(progs);
		}

		int k = 0;
		while(k < input.size())
		{
			if(input[k] == 's')
			{
				k++;

				std::string snum;
				while(isdigit(input[k]))
				{
					snum += input[k];
					k++;
				}

				int num = std::stoi(snum);
				std::rotate(progs.begin(), progs.end() - num, progs.end());
			}
			else if(input[k] == 'x')
			{
				k++;

				std::string a;
				while(isdigit(input[k]))
				{
					a += input[k];
					k++;
				}

				assert(input[k] == '/');
				k++;

				std::string b;
				while(isdigit(input[k]))
				{
					b += input[k];
					k++;
				}

				std::swap(progs[std::stoi(a)], progs[std::stoi(b)]);
			}
			else if(input[k] == 'p')
			{
				k++;
				char a = input[k];
				k += 2;
				char b = input[k];
				k++;

				auto x = std::find(progs.begin(), progs.end(), a);
				auto y = std::find(progs.begin(), progs.end(), b);

				auto tmp = *x;
				*x = *y;
				*y = tmp;
			}
			else if(input[k] == '\n')
			{
				k++;
			}

			if(input[k] == ',')
				k++;
		}

		fprintf(stderr, "%d - %s\n", step, progs.c_str());
	}


	printf("\n");
	for(auto c : progs)
		printf("%c", c);

	printf("\n");
}














