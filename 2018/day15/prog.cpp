// prog.cpp
// Copyright (c) 2017, zhiayang@gmail.com
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

struct entity
{
	// 0 = empty, 1 = wall, 2 = elf, 3 = goblin
	int kind = 0;
	int hp = 0;

	int x = 0;
	int y = 0;

	bool is_alive = true;
};

using map_t = std::map<std::pair<int, int>, entity*>;
using v2 = std::pair<int, int>;

#define START_HEALTH 200

int dim_x = 0;
int dim_y = 0;

entity* get(map_t& map, int x, int y)
{
	assert(x < dim_x && y < dim_y);
	return map[{ x, y }];
}

bool empty(map_t& map, int x, int y)
{
	assert(x < dim_x && y < dim_y);
	return map[{ x, y }]->kind == 0;
}

std::vector<entity*> surroundings(map_t& map, int x, int y, bool require_empty)
{
	std::vector<entity*> ret;

	if(x > 0)
	{
		auto e = get(map, x - 1, y);
		if(!(require_empty && e->kind != 0))
			ret.push_back(e);
	}
	if(y > 0)
	{
		auto e = get(map, x, y - 1);
		if(!(require_empty && e->kind != 0))
			ret.push_back(e);
	}
	if(x < (dim_x - 1))
	{
		auto e = get(map, x + 1, y);
		if(!(require_empty && e->kind != 0))
			ret.push_back(e);
	}
	if(y < (dim_y - 1))
	{
		auto e = get(map, x, y + 1);
		if(!(require_empty && e->kind != 0))
			ret.push_back(e);
	}

	return ret;
}

bool allied(entity* a, entity* b)
{
	// hackish -- walls and empty bits are always your ally
	// so you don't attack them
	if(a->kind == 2 && b->kind == 3) return false;
	if(a->kind == 3 && b->kind == 2) return false;

	return true;
}

int distance(entity* a, entity* b)
{
	return std::abs(a->x - b->x) + std::abs(a->y - b->y);
}

int distance2(v2 a, v2 b)
{
	return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

int linedist(const v2& a, const v2& b)
{
	return (int) std::sqrt(std::pow(a.first - b.first, 2) + std::pow(a.second - b.second, 2));
}

bool inreadingorder(entity* a, entity* b)
{
	if(a->y > b->y)         return false;
	else if(a->y == b->y)   return a->x < b->x;
	else                    return true;
}

bool inreadingorder2(const v2& a, const v2& b)
{
	if(a.second > b.second)         return false;
	else if(a.second == b.second)   return a.first < b.first;
	else                            return true;
}


// deletes the fucker from existence.
void kill(map_t& map, entity* e)
{
	// don't kill walls or something stupid
	assert(e->kind == 2 || e->kind == 3);

	// replace the entity with a hole

	auto spot = new entity();
	spot->kind = 0;
	spot->x = e->x;
	spot->y = e->y;

	map[{ e->x, e->y }] = spot;
}

void moveto(map_t& map, entity* e, int x, int y)
{
	// 'kill' the entity, which replaces the spot on the map with a blank space
	// the pointer is still valid though -- see above.

	// tfm::printfln("entity moving from (%d, %d) to (%d, %d)", e->x, e->y, x, y);
	kill(map, e);

	// make a new entity.
	auto spot = map[{ x, y }];
	assert(spot->kind == 0);

	// just replace it.
	map[{ x, y }] = e;
	e->x = x;
	e->y = y;
}


std::pair<std::vector<v2>, int> findallpaths(map_t& map, const v2& from, const std::set<v2>& targets)
{
	std::set<v2> seen;
	std::deque<std::pair<v2, int>> queue = { { from, 0 } };

	int mindist = INT_MAX;
	std::vector<v2> feasible_targets;

	while(queue.size() > 0)
	{
		auto [ n, dist ] = queue.front();
		queue.pop_front();

		if(seen.find(n) != seen.end())
			continue;

		seen.insert(n);

		if(dist > mindist)
			return { feasible_targets, mindist };

		if(targets.find(n) != targets.end())
		{
			mindist = dist;
			feasible_targets.push_back(n);
		}

		auto neighbours = surroundings(map, n.first, n.second, /* require empty */ true);

		for(auto neigh : neighbours)
			queue.push_back({ v2(neigh->x, neigh->y), dist + 1 });
	}

	return { feasible_targets, mindist };
}






// note: this takes a list of enemies!
// returns true if we moved.
bool move(map_t& map, entity* self, std::vector<entity*>& enemies)
{
	v2 final_target;
	int final_dist = 0;
	{
		// get all possible targets
		std::set<v2> all_targets;
		{
			for(auto enemy : enemies)
			{
				// get the empty spots only.
				auto surr = surroundings(map, enemy->x, enemy->y, /* require empty */ true);
				for(auto e : surr)
					all_targets.insert(v2(e->x, e->y));
			}
		}

		auto [ feasible_targets, target_dist ] = findallpaths(map, v2(self->x, self->y), all_targets);

		if(feasible_targets.empty())
			return false;

		// get the target in reading order
		final_target = util::sort(feasible_targets, [](const v2& a, const v2& b) -> bool {
			return inreadingorder2(a, b);
		})[0];

		final_dist = target_dist;
	}


	v2 final_move;
	{
		// get our neighbours.
		auto neighs = util::sort(surroundings(map, self->x, self->y, /* require empty */ true), inreadingorder);
		for(auto neigh : neighs)
		{
			auto [ ts, dists ] = findallpaths(map, v2(neigh->x, neigh->y), { final_target });
			if(dists == final_dist - 1)
			{
				final_move = v2(neigh->x, neigh->y);
				break;
			}
		}
	}

	moveto(map, self, final_move.first, final_move.second);
	return true;
}









// note: this takes a list of enemies -- that are in range!!
// it also takes a ref to players -- to erase a guy.
// returns true if we killed anyone.
bool attack(map_t& map, entity* self, std::vector<entity*>& enemies, std::vector<entity*>& players, int elf_atk, int gob_atk, entity** e_killed)
{
	if(enemies.empty())
		return false;

	// sort by hp first, then by reading order for ties.
	std::sort(enemies.begin(), enemies.end(), [](auto a, auto b) -> bool {
		if(a->hp == b->hp)
			return inreadingorder(a, b);

		else
			return a->hp < b->hp;
	});

	// attack the first guy
	auto enemy = enemies[0];

	// if the enemy is an elf, we use gob atk.
	if(enemy->kind == 2)    enemy->hp -= gob_atk;
	else                    enemy->hp -= elf_atk;


	if(enemy->hp <= 0)
	{
		enemy->hp = 0;

		kill(map, enemy);
		*e_killed = enemy;

		return true;
	}

	return false;
}

bool turn(map_t& map, entity* self, std::vector<entity*>& players, bool* changed, int elf_atk, int gob_atk, entity** e_killed)
{
	// make a list of enemies.
	std::vector<entity*> enemies = util::filter(players, [self](const auto e) -> bool {
		return !allied(self, e) && e->hp > 0;
	});

	if(enemies.empty())
		return false;

	// check our surroundings to see if we're in range of an enemy.
	// if there are no enemies in range, then we move, else we attack.
	{
		auto in_range = util::filter(surroundings(map, self->x, self->y, /* require empty */ false), [self](auto e) -> bool {
			return !allied(self, e);
		});

		if(!in_range.empty())
		{
			*changed |= attack(map, self, in_range, players, elf_atk, gob_atk, e_killed);
		}
		else
		{
			*changed |= move(map, self, enemies);

			// then attack again!
			auto in_range = util::filter(surroundings(map, self->x, self->y, /* require empty */ false), [self](auto e) -> bool {
				return !allied(self, e);
			});

			if(!in_range.empty())
				*changed |= attack(map, self, in_range, players, elf_atk, gob_atk, e_killed);
		}
	}

	return true;
}

bool simulate(map_t& map, std::vector<entity*>& players, bool* any_change, int elf_atk, bool* elves_died)
{
	const int gob_atk = 3;


	// sort in 'reading order'
	std::sort(players.begin(), players.end(), inreadingorder);

	std::set<entity*> corpses;

	for(auto e : players)
	{
		if(e->is_alive)
		{
			entity* killed = 0;
			auto q = turn(map, e, players, any_change, elf_atk, gob_atk, &killed);
			if(!q) return false;

			if(killed)
			{
				killed->is_alive = false;
				if(killed->kind == 2)
				{
					*elves_died = true;

					// if elf_atk is > 3, then we're doing part 2 -- early break.
					if(elf_atk > 3) return true;
				}
			}
		}
	}

	players = util::filter(players, [&corpses](auto e) -> bool {
		return std::find(corpses.begin(), corpses.end(), e) == corpses.end();
	});

	return true;
}


std::string getstate(map_t& map)
{
	std::string s;

	for(int y = 0; y < dim_y; y++)
	{
		std::vector<entity*> hps;
		for(int x = 0; x < dim_x; x++)
		{
			auto e = map[{ x, y }];
			if(e->kind == 0) s += ".";
			if(e->kind == 1) s += "#";
			if(e->kind == 2) s += "E", hps.push_back(e);
			if(e->kind == 3) s += "G", hps.push_back(e);
		}

		for(auto e : hps) s += tfm::format("  %s(%d)", e->kind == 2 ? "E" : "G", e->hp);

		s += "\n";
	}

	return s;
}

std::pair<map_t, std::vector<entity*>> initialise(const std::vector<std::string>& input)
{
	map_t map;
	std::vector<entity*> players;

	for(int y = 0; y < dim_y; y++)
	{
		auto l = input[y];

		for(int x = 0; x < dim_x; x++)
		{
			auto e = new entity();
			map[{ x, y }] = e;

			e->x = x;
			e->y = y;

			if(l[x] == '.') e->kind = 0, e->hp = 0;
			if(l[x] == '#') e->kind = 1, e->hp = 0;
			if(l[x] == 'E') e->kind = 2, e->hp = START_HEALTH, e->is_alive = true, players.push_back(e);
			if(l[x] == 'G') e->kind = 3, e->hp = START_HEALTH, e->is_alive = true, players.push_back(e);
		}
	}

	return { map, players };
}

void destroy(map_t& map)
{
	for(int y = 0; y < dim_y; y++)
	{
		for(int x = 0; x < dim_x; x++)
		{
			delete map[{ x, y }];
		}
	}
}

int main()
{
	std::vector<std::string> input = {
		// 261855
		"################################",
		"#########.####...#####.#########",
		"#####...#G...#.G.##.#...##.##..#",
		"####...G####.G....#..........E.#",
		"#####..#######.................#",
		"#####..###########.............#",
		"#GG...############.............#",
		"#...#.#.##..######..........#..#",
		"##............#G.#..E.......####",
		"##.G........#....#.........#####",
		"###....G...................#####",
		"###............G.....G.......###",
		"#####.....#..G#####..........###",
		"####..#......#######G...#.E..E##",
		"####G##.G...#########.#.......##",
		"###..###....#########...E....###",
		"##...G......#########.E...######",
		"##G.........#########......#####",
		"##...#.G....#########.#...######",
		"##...#.......#######E.##########",
		"####.#........#####...##########",
		"#######............E..##########",
		"####..#...........E#############",
		"##...G#...........##############",
		"##........#.......##############",
		"#####G..###..E..################",
		"##########......################",
		"##########.....#################",
		"#########......#################",
		"###########.....################",
		"###########...##################",
		"################################",

		// 216270
		// "################################",
		// "######......###...##..##########",
		// "######....#G###G..##.G##########",
		// "#####...G##.##.........#########",
		// "##....##..#.##...........#######",
		// "#....#G.......##.........G.#####",
		// "##..##GG....G.................##",
		// "##.......G............#.......##",
		// "###.....G.....G#......E.......##",
		// "##......##....................##",
		// "#.....####......G.....#...######",
		// "#.#########.G....G....#E.#######",
		// "###########...#####......#######",
		// "###########..#######..E.......##",
		// "###########.#########......#.###",
		// "########..#.#########.........##",
		// "#######G....#########........###",
		// "##.##.#.....#########...EE#..#.#",
		// "#...GG......#########.#...##..E#",
		// "##...#.......#######..#...#....#",
		// "###.##........#####......##...##",
		// "###.........................#..#",
		// "####.............##........###.#",
		// "####............##.........#####",
		// "####..##....###.#...#.....######",
		// "########....###..............###",
		// "########..G...##.###...E...E.###",
		// "#########...G.##.###.E....E.####",
		// "#########...#.#######.......####",
		// "#############..########...######",
		// "##############.########.########",
		// "################################",

		// 27730
		// "#######",
		// "#.G...#",
		// "#...EG#",
		// "#.#.#G#",
		// "#..G#E#",
		// "#.....#",
		// "#######",


		// 36334
		// "#######",
		// "#G..#E#",
		// "#E#E.E#",
		// "#G.##.#",
		// "#...#E#",
		// "#...E.#",
		// "#######",

		// 39514
		// "#######",
		// "#E..EG#",
		// "#.#G.E#",
		// "#E.##E#",
		// "#G..#.#",
		// "#..E#.#",
		// "#######",

		// 27755
		// "#######",
		// "#E.G#.#",
		// "#.#G..#",
		// "#G.#.G#",
		// "#G..#.#",
		// "#...E.#",
		// "#######",

		// 28944
		// "#######",
		// "#.E...#",
		// "#.#..G#",
		// "#.###.#",
		// "#E#G#G#",
		// "#...#G#",
		// "#######",

		// 18740
		// "#########",
		// "#G......#",
		// "#.E.#...#",
		// "#..##..G#",
		// "#...##..#",
		// "#...#...#",
		// "#.G...G.#",
		// "#.....G.#",
		// "#########",
	};

	// it's always a square.
	dim_x = input[0].size();
	dim_y = input.size();

	for(int elf_atk = 3;; elf_atk++)
	{
		auto [ map, players ] = initialise(input);

		int rounds = 0;
		bool any_change = false;
		bool elves_died = false;

		while(true)
		{
			fprintf(stderr, "\r                                   \ratk %d - round %d          ", elf_atk, rounds);

			bool any_enemies = simulate(map, players, &any_change, elf_atk, &elves_died);
			if(!any_enemies) break;

			rounds++;

			if(elves_died && elf_atk > 3) break;
		}

		if(elf_atk == 3 || !elves_died)
		{
			// find the total hitpoints.
			int hps = 0;
			for(auto p : players)
				hps += p->hp;

			tfm::printfln("\npart %d: %d x %d = %d (with elf_atk = %d)", elf_atk == 3 ? 1 : 2, rounds, hps, rounds * hps, elf_atk);
			if(!elves_died) break;
		}

		destroy(map);
	}
}











