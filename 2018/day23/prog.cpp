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

		// new, supposedly more robust attempt
		// DOES NOT WORK, I GIVE UP
		# if 0
		struct box_t
		{
			box_t(const v3& a, const v3& b) : p0(a), p1(b) { }

			v3 p0;
			v3 p1;
			int count = 0;
		};

		// this way the box will always be a cube.
		// makes my life easier.

		// 47141479 / 980
		std::deque<box_t> queue = { box_t(v3(minx, miny, minz), v3(minx + maxdiff, miny + maxdiff, minz + maxdiff)) };

		while(queue.size() > 0)
		{
			auto bb = queue.front();
			queue.pop_front();

			int64_t x0 = bb.p0.x;
			int64_t y0 = bb.p0.y;
			int64_t z0 = bb.p0.z;

			int64_t dx = std::abs(bb.p0.x - bb.p1.x);
			int64_t dy = std::abs(bb.p0.y - bb.p1.y);
			int64_t dz = std::abs(bb.p0.z - bb.p1.z);

			if(dx > 0)
			{
				tfm::printfln("\nbig box: (%d, %d, %d) - (%d, %d, %d)  /  d(%d, %d, %d)", bb.p0.x, bb.p0.y, bb.p0.z, bb.p1.x, bb.p1.y, bb.p1.z, dx, dy, dz);

				// split the bigbox into 8 smaller boxes.
				// looking at a box straight on, with left-x/right+x, up+y/down-y, in+z/out-z
				std::vector<box_t> boxes = {
					// bottom layer, left side, closer to viewer
					box_t(
						v3(x0,                  y0,                 z0),
						v3(x0 + (dx / 2),       y0 + (dy / 2),      z0 + (dz / 2))
					),

					// bottom layer, left side, further from viewer
					box_t(
						v3(x0,                  y0,                 z0 + 0 + (dz / 2)),
						v3(x0 + (dx / 2),       y0 + (dy / 2),      z0 + dz)
					),

					// bottom layer, right side, closer to viewer
					box_t(
						v3(x0 + 0 + (dx / 2),   y0,                 z0),
						v3(x0 + dx,             y0 + (dy / 2),      z0 + (dz / 2))
					),

					// bottom layer, right side, further
					box_t(
						v3(x0 + 0 + (dx / 2),   y0,                 z0 + 0 + (dz / 2)),
						v3(x0 + dx,             y0 + (dy / 2),      z0 + dz)
					),


					// top layer, left side, closer to viewer
					box_t(
						v3(x0,                  y0 + 0 + (dy / 2),  z0),
						v3(x0 + (dx / 2),       y0 + dy,            z0 + (dz / 2))
					),

					// top layer, left side, further from viewer
					box_t(
						v3(x0,                  y0 + 0 + (dy / 2),  z0 + 0 + (dz / 2)),
						v3(x0 + (dx / 2),       y0 + dy,            z0 + dz)
					),

					// top layer, right side, closer to viewer
					box_t(
						v3(x0 + 0 + (dx / 2),   y0 + 0 + (dy / 2),  z0),
						v3(x0 + dx,             y0 + dy,            z0 + (dz / 2))
					),

					// top layer, right side, further
					box_t(
						v3(x0 + 0 + (dx / 2),   y0 + 0 + (dy / 2),  z0 + 0 + (dz / 2)),
						v3(x0 + dx,             y0 + dy,            z0 + dz)
					),
				};


				for(auto& box : boxes)
				{
					// get how many bots are in range of this box.
					int bot_count = 0;
					for(const auto& bot : bots)
					{
						// for each bot, get its min and max of each coord.
						int64_t bx0 = bot.pos.x - bot.range;
						int64_t by0 = bot.pos.y - bot.range;
						int64_t bz0 = bot.pos.z - bot.range;

						int64_t bx1 = bot.pos.x + bot.range;
						int64_t by1 = bot.pos.y + bot.range;
						int64_t bz1 = bot.pos.z + bot.range;

						// check if either b0 or b1 are within range, but for all coords.
						bool conx = (bx0 < box.p0.x && box.p0.x < bx1) || (bx0 < box.p1.x && box.p1.x < bx1);
						bool cony = (by0 < box.p0.y && box.p0.y < by1) || (by0 < box.p1.y && box.p1.y < by1);
						bool conz = (bz0 < box.p0.z && box.p0.z < bz1) || (bz0 < box.p1.z && box.p1.z < bz1);

						if(conx && cony && conz)
						{
							bot_count++;
						}
					}

					// tfm::printfln("count = %d", bot_count);
					box.count = bot_count;
				}


				// insert the new boxes into the queue.
				for(auto b : boxes)
				{
					tfm::printfln("box: (%d, %d, %d) - (%d, %d, %d)", b.p0.x, b.p0.y, b.p0.z, b.p1.x, b.p1.y, b.p1.z);
					queue.push_back(b);
				}

				std::sort(queue.begin(), queue.end(), [](const box_t& a, const box_t& b) -> bool {
					// sort by (1. largest bot count), then (2. smallest dist to origin)
					if(b.count < a.count)
					{
						return true;
					}
					else if(b.count == a.count)
					{
						return dist(a.p0, v3()) < dist(b.p0, v3());
					}
					else
					{
						return false;
					}
				});
			}


			if(dx == 1 || dy == 1 || dz == 1)
			{
				// print and quit. we already sorted it
				auto best = queue[0];
				tfm::printfln("part 2: best spot = (%d, %d, %d), %d from origin, %d bots in range",
					best.p0.x, best.p0.y, best.p0.z, dist(best.p0, v3()), best.count);

				break;
			}
		}

		#else

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

		#endif
	}
}















