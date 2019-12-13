// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"

#include <array>
#include <numeric>

struct Moon
{
	Moon(int x, int y, int z) : pos(x, y, z) { }

	v3 pos;
	v3 vel;

	int get_energy() const
	{
		return (std::abs(this->pos.x) + std::abs(this->pos.y) + std::abs(this->pos.z))
			*  (std::abs(this->vel.x) + std::abs(this->vel.y) + std::abs(this->vel.z));
	}
};

struct SeenHolder
{
	std::set<std::array<v2, 4>> x;
	std::set<std::array<v2, 4>> y;
	std::set<std::array<v2, 4>> z;

	size_t x_rep = 0;
	size_t y_rep = 0;
	size_t z_rep = 0;
};

static void simulate_step(size_t step, std::vector<Moon>& moons, SeenHolder* seens)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(i == j) continue;

			auto& m1 = moons[i];
			auto& m2 = moons[j];

			if(m1.pos.x > m2.pos.x)
				m1.vel.x -= 1, m2.vel.x += 1;

			if(m1.pos.y > m2.pos.y)
				m1.vel.y -= 1, m2.vel.y += 1;

			if(m1.pos.z > m2.pos.z)
				m1.vel.z -= 1, m2.vel.z += 1;
		}
	}

	// do position
	for(auto& m : moons)
		m.pos += m.vel;

	if(seens)
	{
		auto sx = std::array<v2, 4> {
			v2(moons[0].pos.x, moons[0].vel.x), v2(moons[1].pos.x, moons[1].vel.x),
			v2(moons[2].pos.x, moons[2].vel.x), v2(moons[3].pos.x, moons[3].vel.x)
		};

		auto sy = std::array<v2, 4> {
			v2(moons[0].pos.y, moons[0].vel.y), v2(moons[1].pos.y, moons[1].vel.y),
			v2(moons[2].pos.y, moons[2].vel.y), v2(moons[3].pos.y, moons[3].vel.y)
		};

		auto sz = std::array<v2, 4> {
			v2(moons[0].pos.z, moons[0].vel.z), v2(moons[1].pos.z, moons[1].vel.z),
			v2(moons[2].pos.z, moons[2].vel.z), v2(moons[3].pos.z, moons[3].vel.z)
		};

		if(seens->x.find(sx) != seens->x.end())
			seens->x_rep = step;

		if(seens->y.find(sy) != seens->y.end())
			seens->y_rep = step;

		if(seens->z.find(sz) != seens->z.end())
			seens->z_rep = step;
	}
}

int main()
{
	std::vector<Moon> orig_moons = {
		Moon(-3, 10, -1),
		Moon(-12, -10, -5),
		Moon(-9, 0, 10),
		Moon(7, -5, -3)
	};

	{
		auto moons = orig_moons;
		util::foreach(util::rangeClosed<size_t>(1, 1000), [&moons](size_t step) {
			simulate_step(step, moons, nullptr);
		});

		auto total = util::sum(util::map(moons, [](const Moon& m) -> int { return m.get_energy(); }));
		zpr::println("part 1: %d", total);
	}

	{
		auto moons = orig_moons;

		// recognise the initial condition.
		SeenHolder seens;
		seens.x.insert({
			v2(moons[0].pos.x, 0), v2(moons[1].pos.x, 0),
			v2(moons[2].pos.x, 0), v2(moons[3].pos.x, 0)
		});

		seens.y.insert({
			v2(moons[0].pos.y, 0), v2(moons[1].pos.y, 0),
			v2(moons[2].pos.y, 0), v2(moons[3].pos.y, 0)
		});

		seens.z.insert({
			v2(moons[0].pos.z, 0), v2(moons[1].pos.z, 0),
			v2(moons[2].pos.z, 0), v2(moons[3].pos.z, 0)
		});

		for(size_t step = 1; step <= SIZE_T_MAX; step++)
		{
			simulate_step(step, moons, &seens);

			// if we haven't seen a repeat for everybody, keep going.
			if(seens.x_rep && seens.y_rep && seens.z_rep)
				break;
		}

		size_t lcm = std::lcm(seens.x_rep, std::lcm(seens.y_rep, seens.z_rep));
		zpr::println("part 2: %d", lcm);
	}
}














