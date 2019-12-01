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

enum class Atk
{
	COLD,
	FIRE,
	SLASHING,
	BLUDGEON,
	RADIATION,
};

struct group_t
{
	group_t(const std::string& fact, int id, int units, int hp, const std::vector<Atk>& immune, const std::vector<Atk>& weak,
		int attackpower, Atk atk, int init)
	{
		this->faction = fact;
		this->id = id;

		this->units = units;
		this->hp = hp;
		this->initiative = init;

		this->attackpower = attackpower;
		this->attack = atk;

		this->weaknesses = weak;
		this->immunities = immune;
	}

	int effpower() const
	{
		return this->units * (this->attackpower + this->boost);
	}

	std::string faction;

	int id = 0;

	// applies *per unit* !
	int hp = 0;
	int attackpower = 0;

	int boost = 0;

	int units = 0;
	int initiative = 0;

	Atk attack;

	std::vector<Atk> immunities;
	std::vector<Atk> weaknesses;

	bool defending = false;
	group_t* attacking = nullptr;
};

int get_potential_damage(group_t* attacker, group_t* defender)
{
	int eff_atk = attacker->effpower();
	if(util::contains(defender->immunities, attacker->attack))
		return 0;

	else if(util::contains(defender->weaknesses, attacker->attack))
		return eff_atk * 2;

	else
		return eff_atk;
}

// returns true if there was a draw
bool simulate(std::vector<group_t*> groups, std::vector<group_t*> friends, std::vector<group_t*> baddies)
{
	// sort the groups by effective power -- but decreasing.
	std::sort(friends.begin(), friends.end(), [](group_t* a, group_t* b) -> bool {
		if(a->effpower() == b->effpower())
			return b->initiative < a->initiative;

		else
			return b->effpower() < a->effpower();
	});

	std::sort(baddies.begin(), baddies.end(), [](group_t* a, group_t* b) -> bool {
		if(a->effpower() == b->effpower())
			return b->initiative < a->initiative;

		else
			return b->effpower() < a->effpower();
	});


	// ok, groups are sorted.
	for(auto& list : { friends, baddies })
	{
		for(auto self : list)
		{
			auto targets = util::filter((self->faction == "immune system" ? baddies : friends), [](group_t* g) -> bool {
				return g->units > 0 && !g->defending;
			});

			if(targets.empty())
				continue;

			// first check if we can deal any damage
			bool found = false;
			for(auto t : targets)
			{
				if(get_potential_damage(self, t) > 0)
				{
					found = true;
					break;
				}
			}

			// if we're a scrub then skip a turn
			if(!found) continue;

			std::sort(targets.begin(), targets.end(), [&self](group_t* a, group_t* b) -> bool {
				// sort first by pot dmg, then by eff-power, then by initiative.

				int admg = get_potential_damage(self, a);
				int bdmg = get_potential_damage(self, b);

				if(admg == bdmg)
				{
					if(a->effpower() == b->effpower())
					{
						// descending!!!!!
						return b->initiative < a->initiative;
					}
					else
					{
						// descending
						return b->effpower() < a->effpower();
					}
				}
				else
				{
					// descending order
					return bdmg < admg;
				}
			});

			for(auto t : targets)
			{
				int dmg = get_potential_damage(self, t);
				// tfm::printfln("%s/%d would deal %d dmg to %s/%d", self->faction, self->id, dmg, t->faction, t->id);
			}

			assert(targets.size() > 0);

			targets.front()->defending = true;
			self->attacking = targets.front();

			// tfm::printfln("%s/%d will attack %s/%d\n", self->faction, self->id, targets.front()->faction, targets.front()->id);
		}
	}

	// now all the things are sorted, re-sort by initiative.
	std::sort(groups.begin(), groups.end(), [](group_t* a, group_t* b) -> bool {
		return b->initiative < a->initiative;
	});


	// tfm::printfln("battle!");
	int totalkills = 0;
	for(auto& self : groups)
	{
		if(!self->attacking)
			continue;

		// ok.
		group_t* target = self->attacking;
		int dmg = get_potential_damage(self, target);
		tfm::printf("%s/%d attacks %s/%d: %d damage", self->faction, self->id, target->faction, target->id, dmg);

		int kills = 0;
		while(true)
		{
			// only deal whole damage. if the current damage left to deal can kill a unit completely,
			// kill it.
			if(dmg >= target->hp)
			{
				kills++;
				target->units--;
				dmg -= target->hp;
			}
			else
			{
				break;
			}

			// subtract regardless.
			if(target->units == 0)
				break;
		}

		tfm::printfln(", %d units killed", kills);
		totalkills += kills;
	}

	// tfm::printfln("done!\n");

	// reset all the defending/attacking stuff.
	for(auto& g : groups)
	{
		g->defending = false;
		g->attacking = nullptr;
	}

	// there was a stalemate
	return totalkills == 0;
}


std::vector<group_t*> getstartstate()
{
	std::vector<group_t*> groups = {

		// new group_t("immune system", 1, 8808, 5616, { Atk::COLD     }, { Atk::RADIATION },  5,      Atk::BLUDGEON,  10),
		// new group_t("immune system", 2, 900, 13511, {               }, { Atk::RADIATION },  107,    Atk::RADIATION, 20),
		// new group_t("immune system", 3, 581, 10346, { Atk::SLASHING }, { Atk::RADIATION },  140,    Atk::FIRE,      14),
		// new group_t("immune system", 4, 57, 9991,   { Atk::SLASHING, Atk::RADIATION, Atk::FIRE }, { Atk::BLUDGEON }, 1690, Atk::FIRE, 4),
		// new group_t("immune system", 5, 4074, 6549, {               }, { Atk::FIRE },       15,     Atk::RADIATION, 2),
		// new group_t("immune system", 6, 929, 5404,  { Atk::BLUDGEON, Atk::RADIATION }, { }, 45,     Atk::FIRE, 16),
		// new group_t("immune system", 7, 2196, 3186, { Atk::RADIATION }, { Atk::FIRE }, 10, Atk::FIRE, 11),
		// new group_t("immune system", 8, 4420, 9691, { Atk::FIRE },{ Atk::RADIATION }, 21, Atk::FIRE, 7),
		// new group_t("immune system", 9, 3978, 2306, { }, { Atk::COLD, Atk::RADIATION }, 4, Atk::FIRE, 12),
		// new group_t("immune system", 10, 1284, 4487, { }, { Atk::RADIATION, Atk::BLUDGEON }, 32, Atk::SLASHING, 19),


		// new group_t("infection", 1, 4262, 23427, { Atk::FIRE }, {Atk::SLASHING }, 9, Atk::SLASHING, 8),
		// new group_t("infection", 2, 217, 9837, { }, {Atk::BLUDGEON}, 73, Atk::BLUDGEON, 1),
		// new group_t("infection", 3, 5497, 33578, { }, { Atk::RADIATION, Atk::COLD }, 11, Atk::SLASHING, 17),
		// new group_t("infection", 4, 866, 41604, { }, { Atk::COLD }, 76, Atk::RADIATION, 15),
		// new group_t("infection", 5, 1823, 19652, { }, { Atk::FIRE, Atk::COLD }, 20, Atk::SLASHING, 13),
		// new group_t("infection", 6, 2044, 23512, { }, { Atk::COLD }, 22, Atk::SLASHING, 9),
		// new group_t("infection", 7, 373, 40861,  {Atk::COLD },  { }, 215, Atk::SLASHING, 18),
		// new group_t("infection", 8, 5427, 43538, {Atk::RADIATION}, {Atk::BLUDGEON}, 15, Atk::SLASHING, 5),
		// new group_t("infection", 9, 3098, 19840, {}, {Atk::BLUDGEON, Atk::COLD}, 12, Atk::RADIATION, 3),
		// new group_t("infection", 10, 785, 14669, {}, {}, 30, Atk::FIRE, 6),

		new group_t("immune system", 1, 17, 5390, { }, { Atk::RADIATION, Atk::BLUDGEON }, 4507, Atk::FIRE, 2),
		new group_t("immune system", 2, 989, 1274, { Atk::FIRE }, { Atk::BLUDGEON, Atk::SLASHING }, 25, Atk::SLASHING, 3),

		new group_t("infection", 1, 801, 4706, { }, { Atk::RADIATION }, 116, Atk::BLUDGEON, 1),
		new group_t("infection", 2, 4485, 2961, { Atk::RADIATION }, { Atk::FIRE, Atk::COLD }, 12, Atk::SLASHING, 4),
	};

	return groups;
}



int main()
{
	{
		auto groups = getstartstate();

		// select targets
		std::vector<group_t*> winningteam;
		while(true)
		{
			std::vector<group_t*> friends;
			std::vector<group_t*> baddies;

			for(auto& g : groups)
			{
				if(g->units == 0) continue;

				if(g->faction == "immune system")
					friends.push_back(g);

				else
					baddies.push_back(g);
			}

			if(friends.empty())
			{
				winningteam = baddies;
				break;
			}
			else if(baddies.empty())
			{
				winningteam = friends;
				break;
			}

			simulate(groups, friends, baddies);
		}

		int sum = 0;
		for(auto g : winningteam)
			sum += g->units;

		tfm::printfln("part 1: %s won with %d units left", winningteam.front()->faction, sum);
	}

	return 0;

	// simple brute force.
	{
		int prev = 0;
		int boost = 0;
		std::vector<group_t*> winning_immune_groups;
		while(true)
		{
			auto groups = getstartstate();
			for(auto g : groups)
			{
				if(g->faction == "immune system")
					g->boost = boost;
			}

			// select targets
			bool draw = false;
			std::vector<group_t*> winningteam;
			while(!draw)
			{
				std::vector<group_t*> friends;
				std::vector<group_t*> baddies;

				for(auto& g : groups)
				{
					if(g->units == 0) continue;

					if(g->faction == "immune system")
						friends.push_back(g);

					else
						baddies.push_back(g);
				}

				if(friends.empty())
				{
					winningteam = baddies;
					break;
				}
				else if(baddies.empty())
				{
					winningteam = friends;
					break;
				}

				draw = simulate(groups, friends, baddies);
			}

			bool died = false;
			if(draw || winningteam.front()->faction == "infection")
				died = true;


			if(!died)
			{
				// go lower
				winning_immune_groups = winningteam;
				break;
			}
			else
			{
				// go higher
				boost += 1;
			}


			// clear the memory please
			for(auto g : groups)
				delete g;
		}

		bool died = false;
		int sum = 0;
		for(auto g : winning_immune_groups)
			sum += g->units;

		tfm::printfln("part 2: %s won with %d units left, after a boost of %d", winning_immune_groups.front()->faction, sum, boost);
	}
}


























	/*
	Immune System:
	8808 units each with 5616 hit points (immune to cold; weak to radiation) with an attack that does 5 bludgeoning damage at initiative 10
	900 units each with 13511 hit points (weak to radiation) with an attack that does 107 radiation damage at initiative 20
	581 units each with 10346 hit points (weak to radiation; immune to slashing) with an attack that does 140 fire damage at initiative 14
	57 units each with 9991 hit points (immune to slashing, radiation, fire; weak to bludgeoning) with an attack that does 1690 fire damage at initiative 4
	4074 units each with 6549 hit points (weak to fire) with an attack that does 15 radiation damage at initiative 2
	929 units each with 5404 hit points (immune to bludgeoning, radiation) with an attack that does 45 fire damage at initiative 16
	2196 units each with 3186 hit points (weak to fire; immune to radiation) with an attack that does 10 fire damage at initiative 11
	4420 units each with 9691 hit points (weak to radiation; immune to fire) with an attack that does 21 fire damage at initiative 7
	3978 units each with 2306 hit points (weak to cold, radiation) with an attack that does 4 fire damage at initiative 12
	1284 units each with 4487 hit points (weak to radiation, bludgeoning) with an attack that does 32 slashing damage at initiative 19

	Infection:
	4262 units each with 23427 hit points (weak to slashing; immune to fire) with an attack that does 9 slashing damage at initiative 8
	217 units each with 9837 hit points (weak to bludgeoning) with an attack that does 73 bludgeoning damage at initiative 1
	5497 units each with 33578 hit points (weak to radiation, cold) with an attack that does 11 slashing damage at initiative 17
	866 units each with 41604 hit points (weak to cold) with an attack that does 76 radiation damage at initiative 15
	1823 units each with 19652 hit points (weak to fire, cold) with an attack that does 20 slashing damage at initiative 13
	2044 units each with 23512 hit points (weak to cold) with an attack that does 22 slashing damage at initiative 9
	373 units each with 40861 hit points (immune to cold) with an attack that does 215 slashing damage at initiative 18
	5427 units each with 43538 hit points (immune to radiation; weak to bludgeoning) with an attack that does 15 slashing damage at initiative 5
	3098 units each with 19840 hit points (weak to bludgeoning, cold) with an attack that does 12 radiation damage at initiative 3
	785 units each with 14669 hit points with an attack that does 30 fire damage at initiative 6
	*/




