// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

int main()
{
	auto input = readFile("input.txt");
	auto lines = split(input, "\n");

	auto min = v3();
	auto max = v3();

	// present = black
	std::unordered_set<v3> tiles;

	for(auto& line : lines)
	{
		auto pos = v3(0, 0, 0);

		while(line.size() > 0)
		{
			if(line.find("ne") == 0)
			{
				line.remove_prefix(2);
				pos.x++;
				pos.z--;
			}
			else if(line.find("se") == 0)
			{
				line.remove_prefix(2);
				pos.z++;
				pos.y--;
			}
			else if(line.find("nw") == 0)
			{
				line.remove_prefix(2);
				pos.z--;
				pos.y++;
			}
			else if(line.find("sw") == 0)
			{
				line.remove_prefix(2);
				pos.x--;
				pos.z++;
			}
			else if(line.find("e") == 0)
			{
				line.remove_prefix(1);
				pos.x++;
				pos.y--;
			}
			else if(line.find("w") == 0)
			{
				line.remove_prefix(1);
				pos.x--;
				pos.y++;
			}
		}

		if(contains(tiles, pos))
			tiles.erase(pos);
		else
			tiles.insert(pos);
	}

	zpr::println("part 1 = {}", tiles.size());

	auto get_adj = [](v3 pos, const std::unordered_set<v3>& bs) -> std::pair<int, std::unordered_set<v3>> {
		int x = 0;
		std::unordered_set<v3> ws;

		std::array<v3, 6> ds = {
			v3( 0, +1, -1), v3(+1,  0, -1), v3(+1, -1,  0),
			v3( 0, -1, +1), v3(-1,  0, +1), v3(-1, +1,  0)
		};

		for(auto& d : ds)
		{
			if(contains(bs, pos + d))
				x++;
			else
				ws.insert(pos + d);
		}

		return { x, ws };
	};

	for(int i = 0; i < 100; i++)
	{
		std::unordered_set<v3> ws;
		std::unordered_set<v3> next;

		for(const auto& p : tiles)
		{
			auto [ adj, w ] = get_adj(p, tiles);
			if(adj == 1 || adj == 2)
				next.insert(p);

			ws.insert(w.begin(), w.end());
		}

		for(const auto& w : ws)
		{
			auto [ adj, _ ] = get_adj(w, tiles);
			if(adj == 2)
				next.insert(w);
		}

		tiles = std::move(next);
	}

	zpr::println("part 2 = {}", tiles.size());
}

