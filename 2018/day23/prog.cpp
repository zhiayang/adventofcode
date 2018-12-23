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

#include "utils.h"
#include "tinyformat.h"

struct bot_t
{
	bot_t() { }
	bot_t(const v3& p, int r) : pos(p), range(r) { }

	v3 pos;
	int64_t range = 0;
};

int64_t dist(const v3& a, const v3& b)
{
	return std::abs((int64_t) a.x - (int64_t) b.x) + std::abs((int64_t) a.y - (int64_t) b.y) + std::abs((int64_t) a.z - (int64_t) b.z);
}




int main()
{
	std::vector<bot_t> bots;

	auto lines = util::readFileLines("input.txt");
	for(const auto& line : lines)
	{
		if(line != "\n")
		{
			int x, y, z, r;
			sscanf(line.c_str(), "pos=<%d,%d,%d>, r=%d", &x, &y, &z, &r);

			bots.push_back(bot_t(v3(x, y, z), r));
		}
	}


	{
		// get the longest-range bot
		bot_t& main_bot = *std::max_element(bots.begin(), bots.end(), [](const bot_t& a, const bot_t& b) -> bool {
			return a.range < b.range;
		});

		std::vector<bot_t*> in_range;

		// check all the bots
		for(auto& bot : bots)
		{
			if(dist(main_bot.pos, bot.pos) <= main_bot.range)
				in_range.push_back(&bot);
		}


		tfm::printfln("part 1: %d bots in range", in_range.size());
	}





	/*
		what this stupid shit does is basically a 3D binary search, but the step between search points is massive, so we get an answer before the
		heat death of the universe.

		in the first iteration the step-size is grossly oversized, but subsequently, the step size is half of the search radius. since this:
			minx = best_pos.x - searchRadius; maxx = best_pos.x + searchRadius;

		is how we calculate the bounds for the next iteration, after we radius/=2, we will search each bounding box 4^3 = 64 times, before halving
		the radius again.

		this process continues ad-infinitum, while recording the current best-bot-count and best-dist-from-origin. once the current radius is 1 (ie
		the it will be 0 in the next iteration), we break, concluding the search.


		fuck this nonsense dude int overflows COST ME SO MUCH TIME
	 */
	{
		int minx = INT_MAX, miny = INT_MAX, minz = INT_MAX;
		int maxx = INT_MIN, maxy = INT_MIN, maxz = INT_MIN;

		for(const auto& b : bots)
		{
			if(b.pos.x < minx) minx = b.pos.x;
			if(b.pos.x > maxx) maxx = b.pos.x;

			if(b.pos.y < miny) miny = b.pos.y;
			if(b.pos.y > maxy) maxy = b.pos.y;

			if(b.pos.z < minz) minz = b.pos.z;
			if(b.pos.z > maxz) maxz = b.pos.z;
		}

		int maxdiff = std::max({ maxx - minx, maxy - miny, maxz - minz });
		int64_t searchRadius = 1;

		// we could just set it to be = maxdiff, but why not just stick with powers of 2 eh
		while(searchRadius < maxdiff)
			searchRadius *= 2;

		int best_count = 0;
		v3 best_pos = v3();

		while(searchRadius >= 1)
		{
			for(int x = minx; x <= maxx; x += searchRadius)
			{
				for(int y = miny; y <= maxy; y += searchRadius)
				{
					for(int z = minz; z <= maxz; z += searchRadius)
					{
						int count = std::count_if(bots.begin(), bots.end(), [x, y, z](const bot_t& b) -> bool {
							return dist(v3(x, y, z), b.pos) <= b.range;
						});

						if(count > best_count || (count == best_count && dist(v3(x, y, z), v3()) < dist(best_pos, v3())))
						{
							best_count = count;
							best_pos = v3(x, y, z);
						}
					}
				}
			}

			minx = best_pos.x - searchRadius; maxx = best_pos.x + searchRadius;
			miny = best_pos.y - searchRadius; maxy = best_pos.y + searchRadius;
			minz = best_pos.z - searchRadius; maxz = best_pos.z + searchRadius;

			searchRadius /= 2;
		}

		tfm::printfln("part 2: best spot = (%d, %d, %d), %d from origin, %d bots in range",
			best_pos.x, best_pos.y, best_pos.z, dist(best_pos, v3()), best_count);
	}
}















