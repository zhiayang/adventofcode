// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include <array>

#include "aoc2.h"
using namespace util;

struct cup_t
{
	cup_t* next;
	size_t number;
};

static constexpr size_t NUM_CUPS = 1'000'000;

static cup_t* move(cup_t* cur, std::unordered_map<size_t, cup_t*>& nums_to_cup, size_t NUM_CUPS)
{
	auto c1 = cur->next;
	auto c2 = c1->next;
	auto c3 = c2->next;

	auto sub1 = [NUM_CUPS](size_t x) -> int {
		return (x < 2) ? NUM_CUPS : (x - 1);
	};

	auto dest = nums_to_cup[sub1(cur->number)];
	while(dest->number == c1->number || dest->number == c2->number || dest->number == c3->number)
		dest = nums_to_cup[sub1(dest->number)];

	cur->next = c3->next;
	c3->next = dest->next;
	dest->next = c1;

	return cur->next;
}

static void setup(size_t NUM_CUPS, str_view input, std::vector<cup_t>& cups, std::unordered_map<size_t, cup_t*>& nums_to_cup)
{
	cups.resize(NUM_CUPS);
	for(size_t i = 0; i < NUM_CUPS; i++)
	{
		size_t num = 0;
		if(i < input.size())
			num = input[i] - '0';
		else
			num = (i + 1);

		cups[i] = cup_t {
			.number = num,
			.next = &cups[(i + 1) % NUM_CUPS]
		};

		nums_to_cup[num] = &cups[i];
	}
}

int main()
{
	auto input = readFile("input.txt");

	{
		std::vector<cup_t> cups;
		std::unordered_map<size_t, cup_t*> nums_to_cup;

		setup(input.size(), input, cups, nums_to_cup);

		cup_t* cur = &cups[0];
		for(size_t n = 0; n < 100; n++)
			cur = move(cur, nums_to_cup, input.size());

		{
			std::string ans;
			auto cup = nums_to_cup[1]->next;
			do {
				ans += '0' + cup->number;
				cup = cup->next;
			} while(cup != nums_to_cup[1]);

			zpr::println("part 1 = {}", ans);
		}
	}


	{
		std::vector<cup_t> cups;
		std::unordered_map<size_t, cup_t*> nums_to_cup;

		setup(1'000'000, input, cups, nums_to_cup);

		cup_t* cur = &cups[0];
		for(size_t n = 0; n < 10'000'000; n++)
			cur = move(cur, nums_to_cup, 1'000'000);

		auto c1 = nums_to_cup[1]->next;
		auto c2 = c1->next;

		zpr::println("part 2 = {}", c1->number * c2->number);
	}
}

