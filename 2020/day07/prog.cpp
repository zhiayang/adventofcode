// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

struct Bag
{
	bool visited = false;

	std::string name;
	std::vector<Bag*> containedBy;
	std::vector<std::pair<int, Bag*>> contains;
};

static int search_up(Bag* root)
{
	int count = 0;
	for(auto& con : root->containedBy)
	{
		if(!con->visited)
			count += 1;

		count += search_up(con);
		con->visited = true;
	}

	return count;
}

static int search_down(Bag* root)
{
	int count = 0;
	for(auto& con : root->contains)
	{
		count += con.first;
		count += con.first * search_down(con.second);
	}

	return count;
}

int main()
{
	// fuck you, topaz.
	auto lines = readFileLines("input.txt");

	std::map<std::string, Bag*> bags;

	auto get_bag = [&bags](str_view name) -> Bag* {
		if(auto it = bags.find(name.str()); it != bags.end())
		{
			return it->second;
		}
		else
		{
			auto bag = new Bag();
			bag->name = name;
			bags.insert({ name.str(), bag });

			return bag;
		}
	};


	for(auto& line : lines)
	{
		auto parts = split(line, "bags contain");

		// drop the fullstop also.
		auto inner_bags = parts[1].trim().drop_last(1);
		auto outer_bag = get_bag(parts[0].trim());

		if(inner_bags != "no other bags")
		{
			auto bag_list = split(inner_bags, ',');
			for(auto inner : bag_list)
			{
				auto x = inner.find("bag");
				inner = inner.take(x).trim();

				auto int_part = inner.take(inner.find(' '));
				auto name_part = inner.drop(int_part.size() + 1).str();

				int count = to_int(int_part);
				auto inner_bag = get_bag(name_part);

				outer_bag->contains.push_back({ count, inner_bag });
				inner_bag->containedBy.push_back(outer_bag);
			}
		}
	}

	zpr::println("part 1 = {}", search_up(get_bag("shiny gold")));
	zpr::println("part 2 = {}", search_down(get_bag("shiny gold")));
}









