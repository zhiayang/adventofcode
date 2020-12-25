// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include <array>

#include "aoc2.h"
using namespace util;

// haha, this is pretty funny
using Deck = std::deque<int>;
using Decks = std::array<Deck, 2>;

static std::deque<int> combat(Decks players)
{
	auto& p1 = players[0];
	auto& p2 = players[1];

	while(!p1.empty() && !p2.empty())
	{
		auto c1 = p1.front();
		auto c2 = p2.front();

		p1.pop_front();
		p2.pop_front();

		if(c1 > c2)
		{
			p1.push_back(c1);
			p1.push_back(c2);
		}
		else if(c2 > c1)
		{
			p2.push_back(c2);
			p2.push_back(c1);
		}
		else
		{
			assert(false && "tie?");
		}
	}

	if(p1.empty())
		return p2;

	else
		return p1;
}

static std::pair<std::deque<int>, int> recursive_combat(Decks decks)
{
	std::set<Decks> seen;

	while(true)
	{
		// player 1 wins by default -- just quit.
		if(seen.find(decks) != seen.end())
			return { decks[0], 0 };

		seen.insert(decks);

		auto c1 = decks[0].front(); decks[0].pop_front();
		auto c2 = decks[1].front(); decks[1].pop_front();

		int winner = 0;
		if(c1 <= decks[0].size() && c2 <= decks[1].size())
		{
			Decks new_decks = {
				Deck(decks[0].begin(), decks[0].begin() + c1),
				Deck(decks[1].begin(), decks[1].begin() + c2)
			};

			std::tie(std::ignore, winner) = recursive_combat(new_decks);
		}
		else
		{
			if(c1 > c2) winner = 0;
			if(c2 > c1) winner = 1;
		}

		decks[winner].push_back(winner == 0 ? c1 : c2);
		decks[winner].push_back(winner == 0 ? c2 : c1);

		// if either one is empty, quit.
		if(decks[0].empty()) { return { decks[1], 1 }; }
		if(decks[1].empty()) { return { decks[0], 0 }; }
	}
}

static int score(const std::deque<int>& deck)
{
	int score = 0;
	{
		size_t i = 0;
		for(auto it = deck.rbegin(); it != deck.rend(); ++it)
			score += (++i) * (*it);
	}

	return score;
}

int main()
{
	auto input = readFile("input.txt");
	auto players = split(input, "\n\n");

	// top is front of queue
	auto parse = [](str_view lines) -> Deck {
		auto d = split(lines, "\n");
		d.erase(d.begin());

		auto t = map(d, to_int);
		return Deck(t.begin(), t.end());
	};

	auto p1 = parse(players[0]);
	auto p2 = parse(players[1]);

	{
		auto winner = combat({ p1, p2 });
		zpr::println("part 1 = {}", score(winner));
	}

	{
		std::set<Decks> seen;
		auto [ winner, _ ] = recursive_combat({ p1, p2 });
		zpr::println("part 2 = {}", score(winner));
	}
}









