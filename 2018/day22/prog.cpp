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
#include <thread>
#include <string>
#include <vector>
#include <fstream>
#include <functional>
#include <unordered_set>

#include "aoc.h"
#include "tinyformat.h"

int cave_depth = 0;

std::map<v2, int> geologic_idxs;
std::map<v2, int> erosion_levels;

int get_geologic(const v2& p, const v2& target);
int get_erosion(const v2& p, const v2& target)
{
	if(auto it = erosion_levels.find(p); it != erosion_levels.end())
		return it->second;

	int geo = get_geologic(p, target);
	auto ret = (geo + cave_depth) % 20183;

	erosion_levels[p] = ret;
	return ret;
}

int get_geologic(const v2& p, const v2& target)
{
	if(auto it = geologic_idxs.find(p); it != geologic_idxs.end())
		return it->second;

	int ret = 0;
	if(p == v2(0, 0))       ret = 0;
	else if(p == target)    ret = 0;
	else if(p.y == 0)       ret = p.x * 16807;
	else if(p.x == 0)       ret = p.y * 48271;
	else                    ret = get_erosion(v2(p.x - 1, p.y), target) * get_erosion(v2(p.x, p.y - 1), target);

	geologic_idxs[p] = ret;
	return ret;
}

enum RockTypes
{
	TYPE_ROCK   = 0,
	TYPE_WET    = 1,
	TYPE_NARROW = 2
};

enum ToolTypes
{
	TOOL_NONE   = 0,
	TOOL_CLIMB  = 1,
	TOOL_TORCH  = 2,
};

int get_type(const v2& p, const v2& target)
{
	// 0 is rock, 1 is wet, 2 is narrow.
	return get_erosion(p, target) % 3;
}



struct node_t
{
	v2 pos;
	int type = 0;
	std::vector<node_t*> adjs;
};

struct state_t
{
	node_t* node = 0;

	int tool = 0;
	int time_elapsed = 0;

	std::vector<std::pair<v2, int>> path;
};

state_t search(state_t start, node_t* target)
{
	std::deque<state_t> queue = { start };
	std::vector<state_t> all_paths;

	std::set<std::pair<v2, int>> seen;

	while(queue.size() > 0)
	{
		auto cur_state = queue.front();
		queue.pop_front();

		auto node = cur_state.node;

		if(node == target)
		{
			if(cur_state.tool != TOOL_TORCH)
				cur_state.time_elapsed += 7;

			all_paths.push_back(cur_state);
			continue;
		}


		if(seen.find({ node->pos, cur_state.tool }) != seen.end())
			continue;

		else
			seen.insert({ node->pos, cur_state.tool });


		for(node_t* neigh : node->adjs)
		{
			int t1 = node->type;
			int t2 = neigh->type;

			std::vector<state_t> tries;
			if(t1 == t2)
			{
				state_t ns = cur_state;
				ns.node = neigh;
				ns.time_elapsed += 1;

				tries.push_back(ns);
			}
			else if(t2 == TYPE_ROCK)
			{
				state_t ns = cur_state;
				ns.node = neigh;

				if(ns.tool == TOOL_NONE)
				{
					ns.time_elapsed += 8;

					auto p1 = ns;
					p1.tool = TOOL_CLIMB;

					auto p2 = ns;
					p2.tool = TOOL_TORCH;

					if(t1 != TYPE_NARROW)
						tries.push_back(p1);

					if(t1 != TYPE_WET)
						tries.push_back(p2);
				}
				else
				{
					auto p1 = ns;
					p1.time_elapsed += 1;
					tries.push_back(p1);
				}
			}
			else if(t2 == TYPE_WET)
			{
				state_t ns = cur_state;
				ns.node = neigh;

				if(ns.tool == TOOL_TORCH)
				{
					// cannot use torch when wet
					ns.time_elapsed += 8;

					// again, we have two possibilities.
					auto p1 = ns;
					p1.tool = TOOL_CLIMB;

					auto p2 = ns;
					p2.tool = TOOL_NONE;

					if(t1 != TYPE_NARROW)
						tries.push_back(p1);

					if(t1 != TYPE_ROCK)
						tries.push_back(p2);
				}
				else
				{
					auto p1 = ns;
					p1.time_elapsed += 1;
					tries.push_back(p1);
				}
			}
			else if(t2 == TYPE_NARROW)
			{
				state_t ns = cur_state;
				ns.node = neigh;

				if(ns.tool == TOOL_CLIMB)
				{
					// cannot use climbing gear in narrow
					ns.time_elapsed += 8;

					// again, we have two possibilities.
					auto p1 = ns;
					p1.tool = TOOL_TORCH;

					auto p2 = ns;
					p2.tool = TOOL_NONE;

					if(t1 != TYPE_WET)
						tries.push_back(p1);

					if(t1 != TYPE_ROCK)
						tries.push_back(p2);
				}
				else
				{
					auto p1 = ns;
					p1.time_elapsed += 1;
					tries.push_back(p1);
				}
			}

			for(auto& t : tries)
			{
				t.path.push_back({ neigh->pos, t.tool });
				queue.push_back(t);
			}
		}

		std::sort(queue.begin(), queue.end(), [](const state_t& p1, const state_t& p2) -> bool {
			return p1.time_elapsed < p2.time_elapsed;
		});
	}

	return *std::min_element(all_paths.begin(), all_paths.end(), [](const state_t& a, const state_t& b) -> bool {
		return a.time_elapsed < b.time_elapsed;
	});
}


















#define MARGIN 50
int main()
{
	// cave_depth = 4848;
	// auto target = v2(15, 700);

	cave_depth = 5355;
	auto target = v2(14, 796);

	std::map<v2, int> map;
	std::map<v2, node_t*> nodes;

	{
		int sum = 0;
		for(int y = 0; y < target.y + MARGIN; y++)
		{
			for(int x = 0; x < target.x + MARGIN; x++)
			{
				int t = get_type(v2(x, y), target);
				auto p = v2(x, y);

				map[p] = t;

				if(x <= target.x && y <= target.y)
					sum += t;

				auto nd = new node_t();
				nd->pos = p;
				nd->type = t;

				nodes[p] = nd;
			}
		}

		tfm::printfln("part 1: risk level = %d", sum);
	}



	{
		for(int y = 0; y < target.y + MARGIN; y++)
		{
			for(int x = 0; x < target.x + MARGIN; x++)
			{
				auto p = v2(x, y);
				auto nd = nodes[p];

				if(x > 0) nd->adjs.push_back(nodes[v2(x - 1, y)]);
				if(y > 0) nd->adjs.push_back(nodes[v2(x, y - 1)]);

				if(x < target.x + MARGIN - 1) nd->adjs.push_back(nodes[v2(x + 1, y)]);
				if(y < target.y + MARGIN - 1) nd->adjs.push_back(nodes[v2(x, y + 1)]);
			}
		}

		state_t start;
		start.node = nodes[v2(0, 0)];
		start.tool = TOOL_TORCH;
		start.time_elapsed = 0;
		start.path = { { v2(0, 0), TOOL_TORCH } };

		state_t st = search(start, nodes[target]);

		tfm::printfln("part 2: time taken = %d minutes", st.time_elapsed);
	}
}



















