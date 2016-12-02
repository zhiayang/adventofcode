// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 20

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <map>
#include <array>
#include <tuple>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>


std::vector<size_t> getDivisors(size_t num)
{
	std::vector<size_t> ret;

	// todo: increase efficiency/speed
	for(size_t i = 1; i < num; i++)
	{
		if(num % i == 0)
			ret.push_back(i);
	}

	ret.push_back(num);
	return ret;
}

size_t gcd(size_t a, size_t b)
{
	while(b != 0)
	{
		std::tie(a, b) = std::make_tuple(b, a % b);
	}
	return a;
}

size_t getlcm(size_t a, size_t b)
{
	size_t c = gcd(a, b);
	return c == 0 ? 0 : a / c * b;
}


size_t getlcm(std::vector<size_t> ns)
{
	if(ns.empty()) return 0;
	else if(ns.size() < 2) return ns.front();

	size_t res = ns.front();
	for(size_t i = 1; i < ns.size(); i++)
		res = getlcm(res, ns[i]);

	return res;
}



int main()
{
	size_t input = 36000000;


	// {
	// 	size_t inp = input / 10;
	// 	std::vector<size_t> factors = getDivisors(inp);

	// 	for(auto f : factors)
	// 	{
	// 		printf("house visited by elf %zu\n", f);
	// 	}

	// 	size_t lcm = getlcm(factors);
	// 	printf("house = %zu\n", lcm);
	// }


	std::unordered_map<size_t, size_t> visited;


	auto thr = [input](size_t curelf) -> void {

		while(true)
		{
			size_t presents = 0;

			for(size_t house = curelf, numh = 0; true; house += curelf)
			{
				if(presents >= input)
				{
					printf("\n\nhouse %zu got at least %zu presents\n", house, input);
					numh++;
				}

				if(numh == 50)
					break;
			}





			// fprintf(stderr, "\r                                    \r%010zu // %010zu", curhouse, presents);
			curelf -= 2;
		}
	};

	// part 1: 831600

	// sane starting point for part 2
	std::thread t1(thr, 831600);
	std::thread t2(thr, 831601);

	t1.join();
	t2.join();
}













