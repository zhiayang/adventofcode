// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include <vector>
#include <string>
#include <fstream>
#include <stdio.h>

int main()
{
	auto f = std::ifstream("input.txt");
	std::vector<long> mods;

	while(f)
	{
		char buf[64] = { 0 };
		f.getline(buf, 64);

		auto s = std::string(buf);
		if(s.empty()) continue;
		mods.push_back(std::stol(s));
	}

	long sum = 0;
	for(long x : mods)
	{
		x /= 3;
		x -= 2;
		sum += x;
	}

	printf("part 1: %ld\n", sum);



	sum = 0;
	for(long x : mods)
	{
		long fuel = (x / 3) - 2;
		while(fuel > 0)
		{
			sum += fuel;
			fuel = (fuel / 3) - 2;
		}
	}

	printf("part 2: %ld\n", sum);
}
