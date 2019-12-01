// prog.cpp
// Copyright (c) 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include "assert.h"

#include <map>
#include <set>
#include <list>
#include <stack>
#include <array>
#include <deque>
#include <string>
#include <vector>
#include <fstream>
#include <functional>
#include <unordered_set>

#include "aoc.h"
#include "tinyformat.h"


using recipe_iter = std::list<int>::iterator;

recipe_iter cw(std::list<int>& marbles, recipe_iter idx)
{
	if(idx == --marbles.end())
		return marbles.begin();

	else
		return ++idx;
}

std::string print(const std::list<int>& marbles, recipe_iter elf1, recipe_iter elf2)
{
	std::string ret;
	for(auto it = marbles.begin(); it != marbles.end(); it++)
	{
		if(it == elf1)      ret += tfm::format("(%d) ", *it);
		else if(it == elf2) ret += tfm::format("[%d] ", *it);
		else                ret += tfm::format(" %d  ", *it);
	}

	return ret;
}


int main()
{
	int input = 768071;

	// part 2: scan the recipe list
	auto inpstr = std::to_string(input);

	auto check = [&inpstr](const std::list<int>& recipes, const std::string& lastfew) -> bool {
		if(inpstr == lastfew)
		{
			tfm::printfln("part 2: '%s' appears after '%d' recipes", inpstr, recipes.size() - inpstr.size());
			return true;
		}

		return false;
	};

	std::list<int> recipes = { 3, 7 };

	recipe_iter elf1 = recipes.begin();
	recipe_iter elf2 = ++recipes.begin();

	std::string lastfew;

	bool quit = false;
	while(!quit)
	{
		int r1 = *elf1;
		int r2 = *elf2;

		int sum = r1 + r2;
		if(sum >= 10)
		{
			recipes.push_back(sum / 10);
			lastfew.push_back((sum / 10) + '0');
			while(lastfew.size() > inpstr.size())
				lastfew.erase(0, 1);

			if(check(recipes, lastfew))
				quit = true;

			recipes.push_back(sum % 10);
			lastfew.push_back((sum % 10) + '0');
			while(lastfew.size() > inpstr.size())
				lastfew.erase(0, 1);

			if(check(recipes, lastfew))
				quit = true;
		}
		else
		{
			recipes.push_back(sum);
			lastfew.push_back(sum + '0');

			while(lastfew.size() > inpstr.size())
				lastfew.erase(0, 1);

			if(check(recipes, lastfew))
				quit = true;
		}

		if(recipes.size() == input + 10)
		{
			auto it = recipes.begin();
			size_t i = 0;

			while(i < input)
			{
				it++;
				i++;
			}

			std::string s;
			for(int i = 0; i < 10; i++)
				s += std::to_string(*(it++));

			tfm::printfln("part 1: %s", s);
		}

		// the elves move.
		for(int i = 0; i < 1 + r1; i++)
			elf1 = cw(recipes, elf1);

		for(int i = 0; i < 1 + r2; i++)
			elf2 = cw(recipes, elf2);
	}
}
























