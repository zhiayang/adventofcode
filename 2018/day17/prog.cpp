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



struct water_t
{
	water_t(int x, int y) : x(x), y(y) { }

	int x;
	int y;
};

int maxx = 0;
int maxy = 0;

char& get(char* map, int x, int y)
{
	return map[x + y*maxx];
}

std::string print(char* map, const std::deque<water_t>& waters, int cx, int cy)
{
	std::string ret;
	for(int y = std::max(0, cy - 15); y < std::min(maxy, cy + 14); y++)
	{
		for(int x = std::max(0, cx - 22); x < std::min(maxx, cx + 82); x++)
		{
			if(x == cx && y == cy)
			{
				ret += tfm::format("%s%s%s", COLOUR_RED_BOLD, map[x + y*maxx], COLOUR_RESET);
			}
			else
			{
				if(waters.size() > 0 && waters.front().x == x && waters.front().y == y)
				{
					ret += tfm::format("%s%s%s", COLOUR_BLUE_BOLD, map[x + y*maxx], COLOUR_RESET);
				}
				else if(std::find_if(waters.begin(), waters.end(), [x, y](const water_t& w) -> bool {
					return w.x == x && w.y == y;
				}) != waters.end())
				{
					ret += tfm::format("%s%s%s", COLOUR_GREEN_BOLD, map[x + y*maxx], COLOUR_RESET);
				}
				else
				{
					ret += map[x + y*maxx];
				}
			}
		}
		ret += "\n";
	}

	ret += "______________________________________\n";
	return ret;
}

void enqueue_front(std::deque<water_t>& waters, int x, int y)
{
	auto it = std::find_if(waters.begin(), waters.end(), [x, y](const water_t& w) -> bool {
		return w.x == x && w.y == y;
	});

	if(it == waters.end())
		waters.push_front(water_t(x, y));
}

void enqueue_back(std::deque<water_t>& waters, int x, int y)
{
	auto it = std::find_if(waters.begin(), waters.end(), [x, y](const water_t& w) -> bool {
		return w.x == x && w.y == y;
	});

	if(it == waters.end())
		waters.push_back(water_t(x, y));
}


int main()
{
	auto lines = util::readFileLines("input.txt");

	std::vector<std::pair<int, int>> clays;

	for(const auto& line : lines)
	{
		if(line == "\n") continue;

		if(line[0] == 'x')
		{
			int x, y0, y1;
			sscanf(line.c_str(), "x=%d, y=%d..%d", &x, &y0, &y1);
			assert(y0 <= y1);

			for(int y = y0; y <= y1; y++)
				clays.push_back({ x, y });
		}
		else
		{
			int y, x0, x1;
			sscanf(line.c_str(), "y=%d, x=%d..%d", &y, &x0, &x1);
			assert(x0 <= x1);

			for(int x = x0; x <= x1; x++)
				clays.push_back({ x, y });
		}
	}

	// set up x from 0 to 2000.
	// y from 0 to the maximum y

	maxx = 2000;
	maxy = INT_MIN;

	int miny = INT_MAX;

	for(auto [ _, y ] : clays)
	{
		if(y > maxy)      maxy = y;
		else if(y < miny) miny = y;
	}

	maxy += 1;
	char* map = new char[maxx * maxy];
	memset(map, ' ', maxx * maxy);
	tfm::printfln("map size: %d x %d", maxx, maxy);

	for(auto& c : clays)
		map[c.first + (maxx * c.second)] = '#';

	std::deque<water_t> waters;
	enqueue_back(waters, 500, 0);

	get(map, 500, 0) = '+';

	while(waters.size() > 0)
	{
		auto& water = waters.front();
		waters.pop_front();

		int x = water.x;
		int y = water.y;

		// check if we're past the limit.
		if(y >= maxy || y + 1 >= maxy) continue;

		// ok we're not. see if there's something below.
		auto& cur = get(map, x, y);
		auto& down = get(map, x, y + 1);
		if(down == '|') continue;

		if(down == ' ')
		{
			// spread down!
			for(int k = y + 1; k < maxy; k++)
			{
				auto& d = get(map, x, k);
				if(d == ' ')
				{
					d = '|';
				}
				else
				{
					enqueue_front(waters, x, k - 1);
					break;
				}
			}
		}
		else if(down == '#' || down == '~')
		{
			{
				auto l = get(map, x - 1, y);
				auto r = get(map, x + 1, y);
				auto u = get(map, x, y - 1);

				if((l == '#' || l == '~') && (r == '#' || r == '~'))
				{
					cur = '~';
					if(u == '|' || u == '-')
						enqueue_front(waters, x, y - 1);

					continue;
				}
			}

			auto check_side = [&map, &waters](int* bound, int y, int start, int end, int dir) ->bool {

				for(int k = start; k != end; k += (1 * dir))
				{
					*bound = k;

					auto& c = get(map, k, y);
					auto& d = get(map, k, y + 1);

					if((c == '#' || c == '~') && (d == '#' || d == '~'))
					{
						return true;
					}
					else if(c == ' ' || c == '|')
					{
						c = '-';
						if(d != '#' && d != '~')
						{
							enqueue_front(waters, k, y);
							return false;
						}
					}
					if(d == '|' || d == ' ')
					{
						return false;
					}
				}

				return false;
			};

			// spread to the sides
			int lbound = 0;
			int rbound = 0;

			bool lsupported = check_side(&lbound, y, x - 1, 0, -1);
			bool rsupported = check_side(&rbound, y, x + 1, maxx, 1);

			for(int k = lbound + 1; k < rbound; k++)
			{
				if(lsupported && rsupported)
					get(map, k, y) = '~';

				if(get(map, k, y - 1) == '|' || get(map, k, y - 1) == '-')
					enqueue_back(waters, k, y - 1);
			}
		}
		else if(down == '|' || down == '-')
		{
			// do nothing
		}
		else
		{
			tfm::printfln("down = '%c'!", down);
			assert(!"should not happen!");
		}
	}


	// count the waters?!
	int water_count = 0;
	int real_water_count = 0;
	for(int y = miny; y < maxy; y++)
	{
		for(int x = 0; x < maxx; x++)
		{
			auto c = get(map, x, y);
			if(util::match(c, '~', '|', '-'))
				water_count++;

			if(c == '~')
				real_water_count++;
		}
	}

	for(auto [ x, y ] : clays)
	{
		if(get(map, x, y) != '#')
			tfm::printfln("(%d, %d) got replaced by '%c'!", x, y, get(map, x, y));
	}

	tfm::printfln("part 1: %d tiles reachable by water", water_count);
	tfm::printfln("part 2: %d tiles of collected water", real_water_count);
}
















