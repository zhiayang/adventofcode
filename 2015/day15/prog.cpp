// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 15

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <map>
#include <deque>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>


struct Ingredient
{
	Ingredient(std::string n, int c, int d, int f, int t, int e) : name(n), capacity(c), durability(d), flavour(f), texture(t), calories(e) { }

	std::string name;
	long capacity;
	long durability;
	long flavour;
	long texture;
	long calories;
};






int main()
{
	size_t duration = 2503;
	std::vector<Ingredient> ilist;


	{
		ilist.push_back(Ingredient("Sugar", 3, 0, 0, -3, 2));
		ilist.push_back(Ingredient("Sprinkles", -3, 3, 0, 0, 9));
		ilist.push_back(Ingredient("Candy", -1, 0, 4, 0, 1));
		ilist.push_back(Ingredient("Chocolate", 0, 0, -2, 2, 8));
	}


	for(long a = 100; a > 0; a--)
	{
		for(long b = 100 - a; b > 0; b--)
		{
			for(long c = 100 - a - b; c > 0; c--)
			{
				if(a + b + c > 100)
					break;

				long d = 100 - a - b - c;
				long score = 1;


				score *= (ilist[0].durability * a) + (ilist[1].durability * b) + (ilist[2].durability * c) + (ilist[3].durability * d);
				if(score < 0)
					score = 0;

				score *= (ilist[0].capacity * a) + (ilist[1].capacity * b) + (ilist[2].capacity * c) + (ilist[3].capacity * d);
				if(score < 0)
					score = 0;

				score *= (ilist[0].flavour * a) + (ilist[1].flavour * b) + (ilist[2].flavour * c) + (ilist[3].flavour * d);
				if(score < 0)
					score = 0;

				score *= (ilist[0].texture * a) + (ilist[1].texture * b) + (ilist[2].texture * c) + (ilist[3].texture * d);
				if(score < 0)
					score = 0;


				long calories = (ilist[0].calories * a) + (ilist[1].calories * b) + (ilist[2].calories * c) + (ilist[3].calories * d);

				if(score > 0 && calories == 500) // part 2: calories == 500
					printf("a = %ld, b = %ld, c = %ld, d = %ld (score = %09ld)\n", a, b, c, d, score);
			}
		}
	}
}


























