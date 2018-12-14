// prog.cpp
// Copyright (c) 2014 - 2017, zhiayang@gmail.com
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

struct plant
{
	plant(int n, plant* l, plant* r) : num(n), left(l), right(r) { }
	~plant()
	{
		if(left) delete left;
		if(right) delete right;
	}

	int num = 0;
	bool planted = false;

	bool nextState = false;

	plant* left = 0;
	plant* right = 0;
};

plant* left(plant* root, int num)
{
	while(num > 0)
	{
		if(!root->left)
			root->left = new plant(root->num - 1, nullptr, root);

		root = root->left;
		num -= 1;
	}

	return root;
}

plant* right(plant* root, int num)
{
	while(num > 0)
	{
		if(!root->right)
			root->right = new plant(root->num + 1, root, nullptr);

		root = root->right;
		num -= 1;
	}

	return root;
}

std::unordered_set<std::string> live_patterns = {
	"##...",
	"..#.#",
	"#..#.",
	"#.##.",
	".#.##",
	".##..",
	"..#..",
	"#..##",
	"#.#..",
	"..###",
	"...#.",
	"##.#.",
	"#.#.#",
	"#####",
	".#.#.",
	".###.",
	"###.#",
};

std::unordered_set<std::string> dead_patterns = {
	// "##.##",
	// "#....",
	// ".####",
	// ".#..#",
	// ".##.#",
	// "####.",
	// "..##.",
	// "##..#",
	// ".#...",
	// "###..",
	// "....#",
	// "#.###",
	// "...##",
	// ".....",
	// "#...#",
};

bool check(plant* p)
{
	// get the state.
	std::string state;
	auto l2 = left(p, 2)->planted;
	auto l1 = left(p, 1)->planted;

	// returns true if we are the second last!
	bool stop = false;
	if(!p->right || !p->right->right)
		stop = true;

	auto r1 = right(p, 1)->planted;
	auto r2 = right(p, 2)->planted;

	if(l2) state += '#';
	else   state += '.';

	if(l1) state += '#';
	else   state += '.';

	if(p->planted)  state += '#';
	else            state += '.';

	if(r1) state += '#';
	else   state += '.';

	if(r2) state += '#';
	else   state += '.';

	if(live_patterns.find(state) != live_patterns.end())
		p->nextState = true;

	else // if(dead_patterns.find(state) != dead_patterns.end())
		p->nextState = false;

	return stop;
}


plant* getState(const std::string& input)
{
	plant* root = new plant(0, nullptr, nullptr);
	{
		plant* cur = root;
		for(char p : input)
		{
			if(p == '#') cur->planted = true;
			else         cur->planted = false;

			if(!cur->right)
				cur->right = new plant(cur->num + 1, cur, nullptr);

			cur = cur->right;
		}
	}

	return root;
}


void print(plant* root)
{
	plant* p = root;
	while(p->left && p->left->num > -4)
		p = p->left;

	int d = p->num - -5;
	while(d > 0)
	{
		printf(" ");
		d--;
	}


	int lnum = 0;
	while(p && p->num < 117)
	{
		if(p->planted) printf("#");
		else           printf(".");

		lnum = p->num;
		p = p->right;
	}

	d = 119 - lnum;
	while(d > 0)
	{
		printf(" ");
		d--;
	}

	printf("\n");
}

plant* setup(const std::string& start)
{
	auto root = getState(start);

	// prime 2 more things to the left.
	root->left = new plant(-1, nullptr, root);
	root->left->left = new plant(-2, nullptr, root->left);

	return root;
}


plant* simulateStep(plant* root)
{
	// find the leftmost node.
	plant* leftmost = root;
	while(leftmost->left)
		leftmost = leftmost->left;

	{
		auto p = leftmost;

		int stop = 0;
		while(p && stop < 3)
		{
			bool s = check(p);

			if(s && stop == 0)
				stop = 1;

			p = p->right;

			if(stop > 0)
				stop++;
		}
	}

	{
		auto p = leftmost;
		while(p)
		{
			if(p->nextState)
				p->planted = true;

			else
				p->planted = false;

			p->nextState = false;
			p = p->right;
		}
	}

	return root;
}

int getTotal(plant* root)
{
	int total = 0;
	{
		// find the leftmost node.
		plant* p = root;
		while(p->left)
			p = p->left;

		while(p)
		{
			if(p->planted)
				total += p->num;

			p = p->right;
		}
	}

	return total;
}

int main()
{
	std::string input = "#.##.#.##..#.#...##...#......##..#..###..##..#.#.....##..###...#.#..#...######...#####..##....#..###";

	{
		auto root = setup(input);
		for(int i = 1; i <= 20; i++)
			simulateStep(root);

		printf("\npart 1: total = %d\n", getTotal(root));
	}


	{
		auto root = setup(input);

		// fast-forward to say 2000
		for(int i = 1; i <= 2000; i++)
			root = simulateStep(root);

		int origtotal = getTotal(root);

		// keep going till we get 100 things in a row with the same diff.

		int prevtot = 0;
		int prevdiff = 0;
		int sameDiffs = 0;
		while(sameDiffs < 100)
		{
			root = simulateStep(root);
			int tot = getTotal(root);

			int d = tot - prevtot;
			prevtot = tot;

			if(sameDiffs == 0 || d == prevdiff)
			{
				sameDiffs++;
			}
			else
			{
				sameDiffs = 0;
				prevdiff = d;
			}
		}

		// ok, now prevdiff is the diff between subsequent generations.
		int diff = prevdiff;
		{
			// the total is the origTotal (at generation 2000) + (50x10^9 - 2000) * diff
			size_t tot = origtotal + (50000000000ULL - 2000) * diff;
			printf("part 2: total = %zu\n", tot);
		}
	}
}



// ##.## => .
// ##... => #
// ..#.# => #
// #.... => .
// #..#. => #
// .#### => .
// .#..# => .
// .##.# => .
// #.##. => #
// ####. => .
// ..##. => .
// ##..# => .
// .#.## => #
// .#... => .
// .##.. => #
// ..#.. => #
// #..## => #
// #.#.. => #
// ..### => #
// ...#. => #
// ###.. => .
// ##.#. => #
// #.#.# => #
// ##### => #
// ....# => .
// #.### => .
// .#.#. => #
// .###. => #
// ...## => .
// ..... => .
// ###.# => #
// #...# => .
