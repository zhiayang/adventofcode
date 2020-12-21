// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

struct Rule
{
	int num = 0;
	char match = 0;

	bool isAlt = 0;
	std::vector<Rule*> subs;
};

/*
	a naive recursive approach fails to work here, since that performs "top-down" recursion: once the first
	matching rule in an Alt is found, it is propagated upwards; if that decision caused any of the rules in the
	higher levels to fail, the entire thing dies -- even if the other branch of the Alt would have caused the
	rule to succeed overall.

	by maintaining a queue of rules to check (and pushing to the front), we perform "bottom-up" recursion; all
	the rules are added to the queue first, then checked, *upwards*. this way, if an Alt branch causes a higher
	rule to fail, control flow is still at the bottom, and the other Alt branch can be tried.

	while the naive top-down solution worked for the given input in part 1, in general it will not work. 例えば:
	0:  10 20
	10: 11 | 11 12
	11: 'a'
	12: 'b'
	20: 'c'

	here, abc is supposed to match, but it will not, because the 'b' was not consumed since the '11 12' branch
	was not entered. thankfully, this kind of expression did not occur in the actual input.

	for part 2, this kind of expression appears directly, 例えば:
	0:  10 20
	10: 11 | 11 10
	11: 'a'
	20: 'b'

	in both cases, the bottom-up solution works.
*/
static bool check(str_view s, std::deque<Rule*> queue)
{
	if(s.empty() && queue.empty())
		return true;

	if(s.empty() != queue.empty())
		return false;

	auto add = [](std::deque<Rule*>& q, Rule* r) -> std::deque<Rule*> {
		auto copy = q;
		copy.push_front(r);
		return copy;
	};

	auto r = queue.front();
	queue.pop_front();

	if(r->match != 0)
	{
		return (s[0] == r->match)
			&& check(s.drop(1), queue);
	}
	else if(r->isAlt)
	{
		for(auto sub : r->subs)
			if(check(s, add(queue, sub)))
				return true;

		return false;
	}
	else
	{
		// iterate in reverse order, since add() always adds to the front.
		for(auto it = r->subs.rbegin(); it != r->subs.rend(); it++)
			queue = add(queue, *it);

		return check(s, queue);
	}
}

int main()
{
	auto input = readFile("input.txt");
	auto parts = split(input, "\n\n");

	std::map<int, Rule*> rules;
	auto get_rule = [&rules](int num) -> Rule* {
		if(rules[num] == nullptr)
		{
			auto r = new Rule();
			r->num = num;

			rules[num] = r;
		}

		return rules[num];
	};

	int counter = 0;
	{
		auto rule_lines = split(parts[0], "\n");
		for(auto& line : rule_lines)
		{
			auto num = to_int(split(line, ":")[0]);
			auto rest = trim(line.drop(line.find(" ")));

			if(rest.find("\"") != std::string::npos)
			{
				auto rule = get_rule(num);
				rule->match = rest.drop(rest.find("\"") + 1).take(1)[0];
			}
			else if(rest.find("|") != std::string::npos)
			{
				std::vector<Rule*> subs;
				for(auto x : split(rest, "|"))
				{
					auto sbs = map(split(trim(x), " "), to_int);
					if(sbs.size() == 1)
					{
						subs.push_back(get_rule(sbs[0]));
					}
					else
					{
						auto s = get_rule(--counter);
						s->subs = map(sbs, get_rule);
						subs.push_back(s);
					}
				}

				auto rule = get_rule(num);
				rule->isAlt = true;
				rule->subs = subs;
			}
			else
			{
				auto xs = map(split(rest, " "), to_int);

				auto rule = get_rule(num);
				rule->isAlt = false;
				rule->subs = map(xs, get_rule);
			}
		}
	}

	{
		int passed = 0;

		auto zero = get_rule(0);
		auto input_lines = split(parts[1], "\n");
		for(auto& line : input_lines)
			if(check(line, { zero }))
				passed++;

		zpr::println("part 1 = {}", passed);
	}

	{
		int passed = 0;
		{
			auto r8 = get_rule(8);
			auto r11 = get_rule(11);
			auto r31 = get_rule(31);
			auto r42 = get_rule(42);

			auto f1 = get_rule(--counter);
			auto f2 = get_rule(--counter);
			auto f3 = get_rule(--counter);

			f1->subs = { r42, r8 };
			f2->subs = { r42, r31 };
			f3->subs = { r42, r11, r31 };

			r8->isAlt = true;
			r8->subs = { r42, f1 };

			r11->isAlt = true;
			r11->subs = { f2, f3 };
		}

		auto zero = get_rule(0);
		auto input_lines = split(parts[1], "\n");
		for(auto& line : input_lines)
			if(check(line, { zero }))
				passed++;

		zpr::println("part 2 = {}", passed);
	}

	// you're welcome, funkschy
	for(auto& [ _, r ] : rules)
		delete r;
}

