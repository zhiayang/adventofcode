// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

int main()
{
	// fuck you, topaz.
	// and fuck you, topaz apologists.

	auto lines = readFileLines("input.txt");
	auto busses = split(lines[1], ",");

	std::map<uint64_t, uint64_t> bus_timings;
	for(size_t i = 0; i < busses.size(); i++)
	{
		if(busses[i] != "x")
			bus_timings[to_int(busses[i])] = i;
	}

	{
		std::pair<int, int64_t> best = { 0, INT64_MAX };
		auto now = to_int(lines[0]);

		for(auto& [ bus, _ ] : bus_timings)
		{
			auto last = now % bus;
			auto next = now + (bus - last);

			if(best.second > next)
				best = { bus, next };
		}

		zpr::println("part 1 = {}", best.first * (best.second - now));
	}


	{
		/*
			because i'm not a genius that knows about fuckin chinese remainder theorem,
			the way this works is simple.

			0. the initial advance is 1; at each loop, advance t by this number.

			1. look at the first two busses; determine the period of them both being correct
				(this requires us to have seen them "pass" twice)

			2. set the advance to this period; this means that every subsequent loop
				iteration will have the first two busses correct already.

			3. increase the number of busses -- now, the first 3. similarly, increment t by the
				current advance until all 3 busses are correct, then set the advance to this period.

			4. repeat until all busses are correct.
		*/

		uint64_t t = 0;
		uint64_t adv = 1;
		uint64_t num_busses = 2;

		uint64_t last_seen = 0;
		while(true)
		{
			bool all_pass = true;

			auto it = bus_timings.begin();
			for(uint64_t n = 0; n < num_busses; n++)
			{
				auto [ bus, time ] = *it;

				bool x = (t + time) % bus == 0;
				all_pass &= x;
				it++;
			}

			if(all_pass)
			{
				if(num_busses == bus_timings.size())
				{
					zpr::println("part 2 = {}", t);
					break;
				}

				if(last_seen != 0)
				{
					adv = t - last_seen;
					zpr::println("period {} = {} ({}, {})", num_busses, adv, t, last_seen);

					num_busses++;
					last_seen = 0;
				}
				else
				{
					last_seen = t;
				}
			}

			t += adv;
		}
	}
}
