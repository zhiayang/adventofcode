// prog.cpp
// Copyright (c) 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include "assert.h"

#include <map>
#include <set>
#include <array>
#include <string>
#include <vector>
#include <unordered_set>

#include "utils.h"
#include "tinyformat.h"

struct coord
{
	coord() : x(0), y(0) { }
	coord(int x, int y) : x(x), y(y) { }

	int x;
	int y;
};

static int _id = 0;
struct point
{
	point() : x(0), y(0) { }
	point(int x, int y) : id(_id++), x(x), y(y) { }

	operator coord() const
	{
		return coord(x, y);
	}

	int id;

	int x;
	int y;

	bool inf = false;
	int area = 0;
};

int dist(const coord& a, const coord& b)
{
	return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

int main()
{
	std::vector<point> input = {
		point(154, 159),
		point(172, 84),
		point(235, 204),
		point(181, 122),
		point(161, 337),
		point(305, 104),
		point(128, 298),
		point(176, 328),
		point(146, 71),
		point(210, 87),
		point(341, 195),
		point(50, 96),
		point(225, 151),
		point(86, 171),
		point(239, 68),
		point(79, 50),
		point(191, 284),
		point(200, 122),
		point(282, 240),
		point(224, 282),
		point(327, 74),
		point(158, 289),
		point(331, 244),
		point(154, 327),
		point(317, 110),
		point(272, 179),
		point(173, 175),
		point(187, 104),
		point(44, 194),
		point(202, 332),
		point(249, 197),
		point(244, 225),
		point(52, 127),
		point(299, 198),
		point(123, 198),
		point(349, 75),
		point(233, 72),
		point(284, 130),
		point(119, 150),
		point(172, 355),
		point(147, 314),
		point(58, 335),
		point(341, 348),
		point(236, 115),
		point(185, 270),
		point(173, 145),
		point(46, 288),
		point(214, 127),
		point(158, 293),
		point(237, 311),
	};


	// find the extreme corners???
	auto min = coord(10000, 10000);
	auto max = coord(-10000, -10000);
	for(const auto& c : input)
	{
		if(c.x < min.x) min.x = c.x;
		if(c.y < min.y) min.y = c.y;

		if(c.x > max.x) max.x = c.x;
		if(c.y > max.y) max.y = c.y;
	}


	// now we should have the bounds, i guess??
	tfm::printfln("bounds are (%d, %d) / (%d, %d)", min.x, min.y, max.x, max.y);

	for(int x = min.x - 100; x <= max.x + 100; x++)
	{
		for(int y = min.y - 100; y <= max.y + 100; y++)
		{
			// brute force this shit
			int best_id = 0;
			int best_dist = 10000000;

			size_t idx = 0;
			for(const auto& claim : input)
			{
				if(auto d = dist(claim, coord(x, y)); d < best_dist)
					best_dist = d, best_id = idx;

				idx++;
			}

			// see if there are overlaps.
			int equals = 0;
			for(const auto& claim : input)
			{
				if(dist(claim, coord(x, y)) == best_dist)
					equals++;
			}

			if(equals == 1)
			{
				input[best_id].area++;

				if(x == min.x || x == max.x || y == min.y || y == max.y)
					input[best_id].inf = true;
			}
		}
	}

	// ok, now count.
	int best_area = 0;
	int best_id = 0;
	for(const auto& c : input)
	{
		if(c.area > best_area && !c.inf)
			best_area = c.area, best_id = c.id;
	}

	tfm::printfln("part 1: best area = %d", best_area);




	int total = 0;
	for(int x = min.x; x <= max.x; x++)
	{
		for(int y = min.y; y <= max.y; y++)
		{
			int total_dist = 0;
			for(const auto& c : input)
				total_dist += dist(c, coord(x, y));

			if(total_dist < 10000)
				total++;
		}
	}

	tfm::printfln("part 2: total region = %d", total);
}
















