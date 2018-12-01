// prog.cpp
// Copyright (c) 2014 - 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <map>
#include <array>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "lexer.cpp"

int main()
{
	struct Node
	{
		Node(const std::string& n, int w) : name(n), weight(w) { }

		std::string name;
		int weight;

		std::vector<Node*> children;
		Node* parent = 0;
	};

	std::map<std::string, Node*> nodes;


	using TT = TokenType;
	auto tokens = getTokens("day07/input.txt");

	for(size_t i = 0; i < tokens.size() && tokens[i] != TT::EndOfFile; i++)
	{
		auto& tok = tokens[i];
		std::string name = tok.str();

		Node* node = 0;
		if(nodes.find(name) != nodes.end())
		{
			node = nodes[name];
		}
		else
		{
			node = new Node(name, 0);
			nodes[name] = node;
		}

		i++;
		if(tokens[i] != TT::LParen) { printf(">> %s: %s\n", name.c_str(), tokens[i].str().c_str()); abort(); }
		i++;

		assert(tokens[i] == TT::Number);
		node->weight = std::stoi(tokens[i].str());

		i++;
		assert(tokens[i] == TT::RParen);
		i++;

		// printf("%s: %d\n", name.c_str(), node->weight);

		if(tokens[i] == TT::Arrow)
		{
			i++;
			while(true)
			{
				auto name = tokens[i].str();
				i++;

				auto n = nodes[name];
				if(n == 0)
				{
					n = new Node(name, 0);
					nodes[name] = n;
				}

				node->children.push_back(n);
				n->parent = node;

				if(tokens[i] == TT::Comma)
					i++;

				else
					break;
			}
		}
	}


	// starting at any arbitrary node, we should be able to find the parent.
	// let's start from 0.
	Node* bottom = 0;
	{
		auto node = nodes.begin()->second;
		while(node->parent)
			node = node->parent;

		bottom = node;
		printf("bottom = %s\n", node->name.c_str());
	}

	assert(bottom);
	{
		std::function<std::pair<int, Node*> (Node* node)> recurse = [&recurse](Node* node) -> std::pair<int, Node*> {
			int sum = node->weight;
			std::vector<int> weights;
			std::map<int, Node*> weightMap;

			for(auto c : node->children)
			{
				auto ret = recurse(c);
				if(ret.second) return { 0, ret.second };

				weights.push_back(ret.first);
				weightMap[ret.first] = c;

				sum += ret.first;
			}

			// here we make the (imho valid) assumption that *only* the invalid weight will occur once;
			// ie. there are always at least 3 children, because otherwise we would not know which is
			// the "correct" one and which is the "wrong" one.
			for(auto w : weights)
			{
				// ever so slightly inefficient as fuck
				if(std::count(weights.begin(), weights.end(), w) == 1)
				{
					auto it = std::find_if_not(weightMap.begin(), weightMap.end(), [&w](auto k) -> bool {
						return k.first == w;
					});

					// proper weight
					int proper = it->first;
					printf("proper = %d, wrong = %d\n", proper, w);

					auto wrong = weightMap[w]; assert(wrong);

					int diff = proper - w;
					wrong->weight += diff;

					return { wrong->weight, wrong };
				}
			}


			return { sum, 0 };
		};

		auto res = recurse(bottom).second;;
		printf("correct weight (%s) = %d\n", res->name.c_str(), res->weight);
	}
}


























