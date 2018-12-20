// prog.cpp
// Copyright (c) 2018, zhiayang
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

#include "utils.h"
#include "tinyformat.h"


v2 explore(std::map<v2, std::set<v2>>& rooms, const v2& cur, char dir)
{
	if(dir == 'N')
	{
		auto rm = v2(cur.x, cur.y - 2);

		rooms[rm].insert(cur);
		rooms[cur].insert(rm);
		return rm;
	}
	else if(dir == 'S')
	{
		auto rm = v2(cur.x, cur.y + 2);

		rooms[rm].insert(cur);
		rooms[cur].insert(rm);
		return rm;
	}
	else if(dir == 'E')
	{
		auto rm = v2(cur.x + 2, cur.y);

		rooms[rm].insert(cur);
		rooms[cur].insert(rm);
		return rm;
	}
	else if(dir == 'W')
	{
		auto rm = v2(cur.x - 2, cur.y);

		rooms[rm].insert(cur);
		rooms[cur].insert(rm);
		return rm;
	}
	else
	{
		tfm::printfln("invalid direction '%c'", dir);
		abort();
	}
}


void branch(std::map<v2, std::set<v2>>& rooms, const v2& _cur, const std::string& path)
{
	auto cur = _cur;

	int indent = 0;
	for(size_t i = 0; i < path.size(); i++)
	{
		if(path[i] == '(')
		{
			indent++;
			int k = i + 1;

			for(; k < path.size(); k++)
			{
				if(path[k] == '(')
					indent++;

				else if(path[k] == ')')
					indent--;

				if(indent == 0)
					break;
			}

			branch(rooms, cur, path.substr(i + 1, k - i - 1));

			i = k;
			continue;
		}
		if(path[i] == '|')
		{
			// reset our position and explore again.
			if(path[i + 1] == ')')
				i++;

			else
				cur = _cur;
		}
		else if(path[i])
		{
			cur = explore(rooms, cur, path[i]);
		}
	}
}


std::vector<v2> search(const v2& from, const std::map<v2, std::set<v2>>& edges, std::vector<v2> path, std::set<v2> seen,
	int minimum, int* at_least_minimum_count)
{
	if(path.size() >= minimum)
		*at_least_minimum_count += 1;

	std::vector<v2> longest = path;
	for(const auto& room : edges.at(from))
	{
		if(seen.find(room) == seen.end())
		{
			seen.insert(room);
			auto p = search(room, edges, path + room, seen, minimum, at_least_minimum_count);

			if(p.size() > longest.size())
				longest = p;
		}
	}

	return longest;
}



int main()
{
	std::string input = util::readFile("input.txt");
	assert(input[0] == '^');
	assert(input.back() == '$');

	std::map<v2, std::set<v2>> connections;
	input = input.substr(1, input.size() - 2);

	auto cur = v2();
	branch(connections, cur, input);

	// get all rooms
	std::vector<v2> rooms;
	for(const auto& [ r, _ ] : connections)
		rooms.push_back(r);

	tfm::printfln("found %d rooms", rooms.size());

	int long_paths = 0;
	auto longest = search(v2(0, 0), connections, { v2(0, 0) }, { v2(0, 0) }, 1001, &long_paths);

	tfm::printfln("part 1: furthest room is %d doors away", longest.size() - 1);
	tfm::printfln("part 2: %d rooms are at least 1000 doors away", long_paths);
}




































