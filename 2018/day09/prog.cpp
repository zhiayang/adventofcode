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

using marble_iter = std::list<int>::iterator;


marble_iter cw(std::list<int>& marbles, marble_iter idx)
{
	if(idx == --marbles.end())
		return marbles.begin();

	else
		return ++idx;
}

marble_iter ccw(std::list<int>& marbles, marble_iter idx)
{
	if(idx == marbles.begin())
		return --marbles.end();

	return --idx;
}

marble_iter insert_after(std::list<int>& marbles, marble_iter it, int x)
{
	return marbles.insert(++it, x);
}

void remove_at(std::list<int>& marbles, const marble_iter& idx)
{
	marbles.erase(idx);
}


std::string print(const std::list<int>& marbles, marble_iter current)
{
	std::string ret;
	for(auto it = marbles.begin(); it != marbles.end(); it++)
	{
		if(it == current)   ret += tfm::format("(%d) ", *it);
		else                ret += tfm::format("%d ", *it);
	}

	return ret;
}







template <int num_players, int num_marbles>
void simulate(std::array<int64_t, num_players>& scores)
{
	std::list<int> marbles = { 0, 2, 1 };

	// tfm::printfln("[-]: (0)");
	// tfm::printfln("[1]: 0 (1)");

	int player = 2;
	marble_iter current = ++marbles.begin();    // these are the starting conditions regardless

	// tfm::printfln("[%d]: %s", player, print(marbles, current));
	player++;

	for(int marble = 3; marble < num_marbles; marble++)
	{
		if(marble % 23 == 0)
		{
			scores[player] += marble;

			// seven times ccw
			auto x = current;
			for(int i = 0; i < 7; i++)
				x = ccw(marbles, x);

			scores[player] += *x;
			current = cw(marbles, x);

			remove_at(marbles, x);
			// current = ccw(marbles, ccw(marbles, current));
		}
		else
		{
			// insert the current marble after 1 pos cw of us
			current = insert_after(marbles, cw(marbles, current), marble);
		}

		// tfm::printfln("[%d]: %s", player, print(marbles, current));

		if(player == num_players - 1)   player = 0;
		else                            player++;
	}
}

template <int num_players>
int64_t max_score(const std::array<int64_t, num_players>& scores)
{
	// find high score
	int64_t max = 0;
	for(int i = 0; i < num_players; i++)
	{
		// tfm::printfln("player %d: %d", i, scores[i]);
		if(scores[i] > max)
			max = scores[i];
	}

	return max;
}



int main()
{
	constexpr int num_players = 419;
	constexpr int num_marbles = 1 + 71052;

	// setup the first 3 so we don't need to deal with edge cases.
	{
		std::array<int64_t, num_players> scores = { };
		simulate<num_players, num_marbles>(scores);

		tfm::printfln("part 1: max score = %d", max_score(scores));
	}

	// well. 100x marbles??
	{
		std::array<int64_t, num_players> scores = { };
		simulate<num_players, 1 + (num_marbles - 1) * 100>(scores);

		tfm::printfln("part 2: max score = %d", max_score(scores));
	}
}



















