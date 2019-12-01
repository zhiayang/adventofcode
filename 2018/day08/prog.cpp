// prog.cpp
// Copyright (c) 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include "assert.h"

#include <map>
#include <set>
#include <stack>
#include <array>
#include <deque>
#include <string>
#include <vector>
#include <fstream>
#include <functional>
#include <unordered_set>

#include "aoc.h"
#include "tinyformat.h"

struct node_t
{
	std::vector<node_t*> children;
	std::vector<int> metadata;
};

template <typename T>
T pop_front(std::deque<T>& x)
{
	auto ret = x.front();
	x.pop_front();

	return ret;


}


static node_t* parseNode(std::deque<int>& input)
{
	int num_children = pop_front(input);
	int num_metadata = pop_front(input);

	auto node = new node_t();
	for(int i = 0; i < num_children; i++)
		node->children.push_back(parseNode(input));

	for(int i = 0; i < num_metadata; i++)
		node->metadata.push_back(pop_front(input));

	return node;
}

template <class UnaryOp>
void traverse(node_t* input, UnaryOp fn)
{
	fn(input);
	for(auto c : input->children)
		traverse(c, fn);
}

int sum(const std::vector<int>& xs)
{
	int ret = 0;
	for(auto i : xs) ret += i;

	return ret;
}

int main()
{
	std::deque<int> input;// = { 2, 3, 0, 3, 10, 11, 12, 1, 1, 0, 1, 99, 2, 1, 1, 2 };
	{
		auto stream = std::ifstream("input.txt", std::ios::in);

		int x = 0;
		while(stream >> x)
			input.push_back(x);
	}

	auto root = parseNode(input);

	{
		// get the total.
		int total_meta = 0;
		traverse(root, [&total_meta](node_t* n) {
			total_meta += sum(n->metadata);
		});

		tfm::printfln("part 1: metadata total = %d", total_meta);
	}

	{
		std::function<int (node_t*)> getValue = [&getValue](node_t* n) -> int {
			if(n->children.empty())
			{
				return sum(n->metadata);
			}
			else
			{
				int total = 0;
				for(int m : n->metadata)
				{
					// 1 refers to first child!
					int child_index = m - 1;

					if(child_index >= 0 && child_index < n->children.size())
						total += getValue(n->children[child_index]);
				}
				return total;
			}
		};

		int total = getValue(root);
		tfm::printfln("part 2: total = %d", total);
	}
}






































