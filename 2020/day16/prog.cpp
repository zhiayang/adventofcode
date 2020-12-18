// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

struct Constraint
{
	std::string name;

	int min1 = 0;
	int max1 = 0;

	int min2 = 0;
	int max2 = 0;

	bool check(int value) const
	{
		return (min1 <= value && value <= max1)
			|| (min2 <= value && value <= max2);
	}
};

static int64_t part1(const std::vector<Constraint>& constrs, std::set<size_t>& wrongs,
	const std::vector<std::vector<int64_t>>& tickets)
{
	int64_t ret = 0;
	for(size_t i = 0; i < tickets.size(); i++)
	{
		for(auto f : tickets[i])
		{
			auto at_least_one = foldl(false, constrs, [f](bool r, auto& c) -> bool {
				return r | c.check(f);
			});

			if(!at_least_one)
			{
				wrongs.insert(i);
				ret += f;
			}
		}
	}

	return ret;
}

static int64_t part2(const std::vector<Constraint>& constrs, const std::set<size_t>& invalids,
	const std::vector<std::vector<int64_t>>& tickets, const std::vector<int64_t>& self)
{
	std::set<int64_t> solved_fields;
	std::map<int64_t, int64_t> field_mapping;

	while(field_mapping.size() < constrs.size())
	{
		for(size_t ci = 0; ci < constrs.size(); ci++)
		{
			// if we found it, skip it.
			if(field_mapping.find(ci) != field_mapping.end())
				continue;

			// there will be one constraint with only one field matching,
			// so narrow down progressively.

			// std::vector<bool> is a fucking pain.
			auto all_pass = std::vector<int>(tickets[0].size(), 1);

			for(size_t ti = 0; ti < tickets.size(); ti++)
			{
				if(invalids.find(ti) != invalids.end())
					continue;

				auto& ticket = tickets[ti];
				for(size_t fn = 0; fn < ticket.size(); fn++)
					all_pass[fn] &= constrs[ci].check(ticket[fn]);
			}

			int64_t map = 0;
			bool found = false;
			for(size_t i = 0; i < all_pass.size(); i++)
			{
				if(solved_fields.find(i) != solved_fields.end())
					continue;

				if(all_pass[i])
				{
					if(found) { found = false; break; }
					else      { found = true; map = i; }
				}
			}

			if(!found)
				continue;

			solved_fields.insert(map);
			field_mapping[ci] = map;
		}
	}

	int64_t answer = 1;
	for(size_t ci = 0; ci < constrs.size(); ci++)
		if(constrs[ci].name.find("departure") == 0)
			answer *= self[field_mapping[ci]];

	return answer;
}

int main()
{
	auto file = readFile("input.txt");
	auto parts = split(file, "\n\n");

	std::vector<Constraint> constrs;
	{
		auto cons = split(parts[0], '\n');
		for(auto& con : cons)
		{
			Constraint c;
			c.name = con.take(con.find(':')).str();
			auto nums = con.drop(con.find(':') + 1).trim();

			c.min1 = to_int(split(split(nums, "or")[0], '-')[0].trim());
			c.max1 = to_int(split(split(nums, "or")[0], '-')[1].trim());
			c.min2 = to_int(split(split(nums, "or")[1], '-')[0].trim());
			c.max2 = to_int(split(split(nums, "or")[1], '-')[1].trim());

			constrs.push_back(std::move(c));
		}
	}

	std::vector<int64_t> self = map(split(split(parts[1], '\n')[1], ','), to_int);
	std::vector<std::vector<int64_t>> tickets;
	{
		auto lines = split(parts[2], '\n');
		lines.erase(lines.begin());

		tickets = map(lines, [](auto& str) -> std::vector<int64_t> {
			return map(split(str, ','), to_int);
		});
	}

	std::set<size_t> invalids;
	zpr::println("part 1 = {}", part1(constrs, invalids, tickets));
	zpr::println("part 2 = {}", part2(constrs, invalids, tickets, self));
}
