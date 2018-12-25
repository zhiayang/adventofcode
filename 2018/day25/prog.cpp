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

#include "utils.h"
#include "tinyformat.h"

struct v4
{
	v4() : x(0), y(0), z(0), w(0) { }
	v4(int x, int y, int z, int w) : x(x), y(y), z(z), w(w) { }

	int x;
	int y;
	int z;
	int w;

	bool touched = false;
	std::vector<v4*> edges;
};

bool operator == (const v4& a, const v4& b)
{
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

bool operator != (const v4& a, const v4& b)
{
	return !(a == b);
}

int dist(const v4& a, const v4& b)
{
	return std::abs(a.x - b.x) + std::abs(a.y - b.y) + std::abs(a.z - b.z) + std::abs(a.w - b.w);
}


void include(std::vector<v4>& constel, v4* pt)
{
	if(pt->touched)
		return;

	pt->touched = true;
	constel.push_back(*pt);

	for(auto& p : pt->edges)
		include(constel, p);
}

int main()
{
	std::vector<std::string> input = util::readFileLines("input2.txt");
	std::vector<v4> coords;

	std::vector<std::vector<v4>> constellations;


	for(const auto& line : input)
	{
		int x, y, z, w;
		sscanf(line.c_str(), "%d,%d,%d,%d", &x, &y, &z, &w);
		coords.emplace_back(x, y, z, w);
	}


	for(auto& pt1 : coords)
	{
		for(auto& pt2 : coords)
		{
			if(pt1 != pt2 && dist(pt1, pt2) <= 3)
			{
				if(std::find(pt1.edges.begin(), pt1.edges.end(), &pt2) == pt1.edges.end())
					pt1.edges.push_back(&pt2);
			}
		}
	}


	for(auto& pt : coords)
	{
		if(!pt.touched)
		{
			constellations.push_back({ });
			include(constellations.back(), &pt);
		}
	}

	tfm::printfln("%d constellations", constellations.size());
}






















