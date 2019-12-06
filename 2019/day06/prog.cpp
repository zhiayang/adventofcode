// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"

#include <set>

struct Object
{
	Object(const std::string& s) : name(s) { }

	std::string name;
	Object* parent = 0;
	std::vector<Object*> orbiters;
};

int main()
{
	std::unordered_map<std::string, Object*> objects;
	Object* COM = new Object("COM");
	objects["COM"] = COM;

	auto lines = util::readFileLines("input.txt");

	util::foreach(lines, [&objects](std::string_view line) {
		auto a = std::string(line.substr(0, line.find(')')));
		auto b = std::string(line.substr(line.find(')') + 1, 6));

		auto get_or_make = [&objects](const std::string& n) -> Object* {
			if(auto it = objects.find(n); it != objects.end())
				return it->second;

			else
				return (objects[n] = new Object(n));
		};

		// b orbits a.
		Object* ao = get_or_make(a);
		Object* bo = get_or_make(b);

		ao->orbiters.push_back(bo);
		bo->parent = ao;
	});

	// do a traversal.
	{
		int totalHeight = 0;
		std::function<void (Object*, int)> traverse = [&totalHeight, &traverse](Object* o, int height) {
			totalHeight += height;
			for(auto child : o->orbiters)
				traverse(child, height + 1);
		};

		traverse(COM, 0);
		zpr::println("part 1: %d", totalHeight);
	}

	{
		// note: subtract 1 cos the starting node isn't part of the path.
		auto cost = util::astarSearch(objects["YOU"]->parent, objects["SAN"]->parent,
			[](Object* x) -> std::vector<Object*> {
				if(x->parent)   return x->orbiters + x->parent;
				else            return x->orbiters;
			}, [](Object* a, Object* b) -> int {
				// it's not grid-based, so we don't have a cheap and good heuristic function. just return 0
				// this degenerates A* to dijkstra.
				return 0;
			}).size() - 1;

		zpr::println("part 2: %d", cost);
	}
}
















