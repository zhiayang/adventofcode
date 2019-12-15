// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"
#include <assert.h>

struct Recipe
{
	std::pair<std::string, int64_t> result;
	std::vector<std::pair<std::string, int64_t>> ingredients;
};


static int64_t get_min_cost(std::unordered_map<std::string, Recipe>& recipes, const std::string& prod,
	int64_t amt, std::unordered_map<std::string, int64_t>& leftovers);

static int64_t get_recipe_cost(std::unordered_map<std::string, Recipe>& recipes, const Recipe& r,
	std::unordered_map<std::string, int64_t>& leftovers, int64_t mult)
{
	int64_t total = 0;

	for(const auto& igt : r.ingredients)
	{
		auto amt = igt.second * mult;
		if(leftovers[igt.first] > 0)
		{
			int64_t needed = std::max(static_cast<int64_t>(0), amt - leftovers[igt.first]);
			leftovers[igt.first] -= (amt - needed);

			if(needed > 0) amt = needed;
			else continue;
		}

		total += get_min_cost(recipes, igt.first, amt, leftovers);
	}

	return total;
}

static int64_t get_min_cost(std::unordered_map<std::string, Recipe>& recipes, const std::string& prod,
	int64_t amt, std::unordered_map<std::string, int64_t>& leftovers)
{
	if(prod == "ORE")
		return amt;

	int64_t min_cost = INT64_MAX;
	auto recipe = recipes[prod];

	{
		int64_t produced = recipe.result.second;
		int64_t multiples = (amt + (produced - 1)) / produced;

		int64_t cost = get_recipe_cost(recipes, recipe, leftovers, multiples);

		min_cost = std::min(min_cost, cost);

		leftovers[prod] += (multiples * produced - amt);
	}

	return min_cost;
}

static int64_t get_min_cost(std::unordered_map<std::string, Recipe>& recipes, const std::string& prod,
	int64_t amt)
{
	std::unordered_map<std::string, int64_t> leftovers;
	return get_min_cost(recipes, prod, amt, leftovers);
}


int main()
{
	std::unordered_map<std::string, Recipe> recipes;

	// the setup.
	util::foreach(util::readFileLines("input.txt"), [&recipes](std::string_view line) {

		auto is = line.substr(0, line.find("=>"));
		auto ps = util::trim(line.substr(line.find("=>") + 2));

		Recipe recipe;

		{
			int c = 0;
			char buf[10];
			sscanf(std::string(ps).c_str(), "%d %s", &c, buf);

			recipe.result = { std::string(buf), c };
		}

		auto igds = util::splitString(is, ',');
		for(auto i : igds)
		{
			int c = 0;
			char buf[10];
			sscanf(std::string(i).c_str(), "%d %s", &c, buf);

			recipe.ingredients.push_back({ std::string(buf), c });
		}

		recipes[recipe.result.first] = recipe;
	});





	{
		auto ore = get_min_cost(recipes, "FUEL", 1);
		zpr::println("part 1: %d", ore);
	}


	{
		// one trillion uwu
		constexpr int64_t max_ore = 1e12;

		int64_t min_fuel = 1;
		int64_t max_fuel = 1048576;
		while(true)
		{
			auto ore = get_min_cost(recipes, "FUEL", max_fuel);

			if(ore < max_ore)   max_fuel *= 2;
			else                break;
		}


		while(true)
		{
			if(min_fuel + 1 >= max_fuel)
			{
				zpr::println("part 2: %d", min_fuel);
				break;
			}

			int64_t fuel = (max_fuel + min_fuel) / 2;
			auto ore = get_min_cost(recipes, "FUEL", fuel);

			if(ore <= max_ore)
				min_fuel = fuel;

			else if(ore > max_ore)
				max_fuel = fuel;
		}
	}
}














