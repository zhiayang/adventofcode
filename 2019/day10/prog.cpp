// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"

#include <numeric>

static constexpr double PI = 3.14159265358979323846264338327950;

int main()
{
	auto input = util::readFile("input.txt");
	std::map<v2, int> asteroids;

	int cx = 0;
	int cy = 0;

	int dim_x = 0;
	for(auto c : input)
	{
		if(c == '#')
			asteroids[v2(cx, cy)] = 1;

		cx++;
		if(c == '\n')
			dim_x = cx - 1, cx = 0, cy++;
	}
	int dim_y = cy + 1;


	auto all = util::mapFilter(util::cartesian(util::rangeOpen(0, dim_x), util::rangeOpen(0, dim_y)),
		[](std::pair<int, int> x) -> v2 {
			return v2(x.first, x.second);
		}, [&asteroids](const v2& x) -> bool {
			return asteroids.find(x) != asteroids.end();
		});

	auto get_angle_between = [](v2 src, v2 dst) -> double {
		// atan2(y, x) -- but y is +ve-up, so negate it.

		int dx = dst.x - src.x;
		int dy = dst.y - src.y;

		auto angle = std::atan2(-dy, dx);

		// fix the angle.
		if(angle >= 0 && angle <= PI/2)
			angle = PI/2 - angle;

		else if(angle > PI/2 && angle <= PI)
			angle = 5*PI/2 - angle;

		else if(angle < 0)
			angle = PI/2 + -angle;

		return angle;
	};


	v2 station = v2();
	{
		int max_asteroids = 0;

		for(auto ast : all)
		{
			auto others = util::groupBy(util::filter(all, [&](auto x) -> bool {
				return ast != x;
			}), [&](auto x) -> double {
				return get_angle_between(ast, x);
			});

			auto cansee = static_cast<int>(others.size());

			if(cansee > max_asteroids)
				max_asteroids = cansee, station = ast;
		}

		zpr::println("part 1: %d", max_asteroids);
	}




	{
		auto groups = util::pairs(util::groupBy(util::filter(all, [&](auto x) -> bool {
			return station != x;
		}), [&](auto x) -> double {
			return get_angle_between(station, x);
		}));

		// sort the groups first by angle, then the asteroids in each group by their distance
		// to the station.
		std::sort(groups.begin(), groups.end(), [](const auto& a, const auto& b) -> bool {
			return a.first < b.first;
		});

		for(auto& g : groups)
		{
			std::sort(g.second.begin(), g.second.end(), [&station](const auto& a, const auto& b) -> bool {
				return util::distance(station, a) < util::distance(station, b);
			});
		}


		v2 alderaan;
		size_t alderaans_destroyed = 0;

		while(alderaans_destroyed < 200)
		{
			for(auto& g : groups)
			{
				if(g.second.size() > 0)
				{
					alderaan = g.second.front();

					g.second.erase(g.second.begin());
					alderaans_destroyed++;
				}

				if(alderaans_destroyed == 200)
					break;
			}
		}

		zpr::println("part 2: %d", alderaan.x * 100 + alderaan.y);
	}
}















