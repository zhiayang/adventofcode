// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"

#include <chrono>
struct timer
{
	using hrc = std::chrono::high_resolution_clock;

	timer() : out(nullptr)              { start = hrc::now(); }
	explicit timer(double* t) : out(t)  { start = hrc::now(); }
	~timer()                            { if(out) *out = static_cast<double>((hrc::now() - start).count()) / 1000000.0; }
	double measure()                    { return static_cast<double>((hrc::now() - start).count()) / 1000000.0; }

	double* out = 0;
	std::chrono::time_point<hrc> start;
};


static int magnitude(const v2& v)
{
	return std::abs(v.x) + std::abs(v.y);
}

static void simulate_step(std::map<v2, int>& path, v2& pos, int& steps, std::string_view step)
{
	int dist = std::stoi(std::string(step.substr(1)));

	std::map<char, v2> dirs = {
		{ 'U', v2(0, -1) },
		{ 'D', v2(0, +1) },
		{ 'L', v2(-1, 0) },
		{ 'R', v2(+1, 0) }
	};

	for(int i = 0; i < dist; i++)
	{
		pos += dirs[step[0]];
		steps++;

		if(path.find(pos) == path.end())
			path[pos] = steps;
	}
}

static std::map<v2, int> simulate_wire(const std::vector<std::string_view>& steps)
{
	std::map<v2, int> path;

	auto pos = v2(0, 0);
	int step_count = 0;

	// setup.
	path[pos] = step_count;

	for(const auto& s : steps)
		simulate_step(path, pos, step_count, s);

	return path;
}


int main()
{
	auto lines = util::readFileLines("input.txt");

	timer t;

	auto path1 = simulate_wire(util::splitString(lines[0], ','));
	auto path2 = simulate_wire(util::splitString(lines[1], ','));

	int best_dist = INT_MAX;
	int best_step = INT_MAX;

	for(const auto& [ v, steps ] : path1)
	{
		if(v != v2(0, 0) && path2.find(v) != path2.end())
		{
			int step_sum = steps + path2[v];
			int distance = magnitude(v);

			if(step_sum < best_step)
				best_step = step_sum;

			if(distance < best_dist)
				best_dist = distance;
		}
	}

	auto time = t.measure();

	zpr::println("part 1: %d", best_dist);
	zpr::println("part 2: %d", best_step);
	zpr::println("time taken: %.1f ms", time);
}








