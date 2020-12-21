// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

int main()
{
	auto input = readFile("input.txt");
	auto lines = split(input, "\n");

	std::vector<std::string> all_ingredients;
	std::map<std::string, std::set<std::string>> allergen_map;

	for(auto& line : lines)
	{
		// split at (
		auto parts = split(line, '(');

		auto ingredients = map(split(trim(parts[0]), ' '), [](auto& a) { return a.str(); });
		auto allergens = map(split(trim(parts[1]).drop(8).drop_last(1), ','), [](auto& a) { return trim(a).str(); });

		// we want duplicates here!
		all_ingredients.insert(all_ingredients.end(), ingredients.begin(), ingredients.end());

		auto ingr_set = std::set(ingredients.begin(), ingredients.end());

		for(auto& alg : allergens)
		{
			if(auto it = allergen_map.find(alg); it != allergen_map.end())
			{
				std::set<std::string> new_set;
				std::set_intersection(it->second.begin(), it->second.end(), ingr_set.begin(), ingr_set.end(),
					std::inserter(new_set, new_set.begin()));

				it->second = std::move(new_set);
			}
			else
			{
				allergen_map[alg] = ingr_set;
			}
		}
	}


	{
		std::set<std::string> suspicious_ingredients;
		for(auto& [ alg, ingr_set ] : allergen_map)
			suspicious_ingredients.insert(ingr_set.begin(), ingr_set.end());

		// count how many times they appear
		int64_t counter = 0;
		for(auto& ingr : all_ingredients)
			if(suspicious_ingredients.find(ingr) == suspicious_ingredients.end())
				counter++;

		zpr::println("part 1 = {}", counter);
	}

	{
		std::vector<std::pair<std::string, std::string>> output;
		while(output.size() < allergen_map.size())
		{
			// eliminate the fuckers... same as day 16 tbh
			for(auto& [ alg, ingr_set ] : allergen_map)
			{
				if(ingr_set.size() == 1)
				{
					auto ingr = *ingr_set.begin();
					output.push_back({ alg, ingr });

					// yeet it from the other allergens. it doesn't actually matter if we also
					// yeet it from our own list.
					for(auto& [ alg1, ingrs1 ] : allergen_map)
						ingrs1.erase(ingr);
				}
			}
		}

		// sort by allergen name
		std::sort(output.begin(), output.end(), [](auto& a, auto& b) -> bool { return a.first < b.first; });

		std::string str;
		for(auto& [ _, i ] : output)
			str += zpr::sprint("{},", i);

		str.pop_back();
		zpr::println("part 2 = {}", str);
	}
}

