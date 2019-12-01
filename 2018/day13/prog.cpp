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

#include "aoc.h"
#include "tinyformat.h"

enum class direction
{
	none,
	right,
	up,
	left,
	down
};

static int id = 0;
struct cart
{
	cart(int x, int y, direction d) : id(::id++), x(x), y(y), dir(d) { }

	int id = 0;

	int x = 0;
	int y = 0;

	// 1 = right, 2 = up, 3 = left, 4 = down
	direction dir;

	// so that the first time it turns it'll go left.
	// 0 = left, 1 = straight, 2 = right
	int prev_turn = 2;

	bool removed = false;
};

int main()
{
	auto lines = util::readFileLines("input.txt");

	std::vector<cart> carts;

	// track: 1 = /   2 = \   3 = +
	// if the thing is 10, 11, 12, or 13, there's a cart on the spot.
	std::map<std::pair<int, int>, int> track;


	for(size_t y = 0; y < lines.size(); y++)
	{
		for(size_t x = 0; x < lines[y].size(); x++)
		{
			// might not work, but for now we ignore the straight tracks.
			if(lines[y][x] == '/')
				track[{ x, y }] = 1;

			else if(lines[y][x] == '\\')
				track[{ x, y }] = 2;

			else if(lines[y][x] == '+')
				track[{ x, y }] = 3;

			else if(lines[y][x] == '>')
			{
				carts.push_back(cart(x, y, direction::right));
				track[{ x, y }] = 10;
			}
			else if(lines[y][x] == '^')
			{
				carts.push_back(cart(x, y, direction::up));
				track[{ x, y }] = 10;
			}
			else if(lines[y][x] == '<')
			{
				carts.push_back(cart(x, y, direction::left));
				track[{ x, y }] = 10;
			}
			else if(lines[y][x] == 'v')
			{
				carts.push_back(cart(x, y, direction::down));
				track[{ x, y }] = 10;
			}
		}
	}



	// tick!
	int tick = 0;
	bool quit = false;
	while(!quit)
	{
		std::sort(carts.begin(), carts.end(), [](const auto& a, const auto& b) -> bool {
			if(a.y > b.y)       return false;
			else if(a.y == b.y) return a.x < b.x;
			else                return true;
		});

		for(auto& cart : carts)
		{
			// skip removed carts.
			if(cart.removed) continue;

			int px = cart.x;
			int py = cart.y;

			int tile = track[{ px, py }];

			assert(tile >= 10 && "?!");

			if(tile == 10)
			{
				switch(cart.dir)
				{
					case direction::up:
						cart.y--;
						break;

					case direction::down:
						cart.y++;
						break;

					case direction::left:
						cart.x--;
						break;

					case direction::right:
						cart.x++;
						break;

					default:
						assert(!"!?!");
				}
			}
			else if(tile == 11)
			{
				// 11 = /
				switch(cart.dir)
				{
					case direction::up:
						cart.dir = direction::right;
						cart.x++;
						break;

					case direction::down:
						cart.dir = direction::left;
						cart.x--;
						break;

					case direction::left:
						cart.dir = direction::down;
						cart.y++;
						break;

					case direction::right:
						cart.dir = direction::up;
						cart.y--;
						break;

					default:
						assert(!"!?!");
				}
			}
			else if(tile == 12)
			{
				// 12 = backslash
				switch(cart.dir)
				{
					case direction::up:
						cart.dir = direction::left;
						cart.x--;
						break;

					case direction::down:
						cart.dir = direction::right;
						cart.x++;
						break;

					case direction::left:
						cart.dir = direction::up;
						cart.y--;
						break;

					case direction::right:
						cart.dir = direction::down;
						cart.y++;
						break;

					default:
						assert(!"!?!");
				}
			}
			else if(tile == 13)
			{
				// 13 = +
				// note: direction is relative to the cart! so it goes left then straight then right,
				// but we need to take that from the POV of the cart.

				switch(cart.dir)
				{
					case direction::up:
						if(cart.prev_turn == 0)      // went left before, so go straight
							cart.y--;

						else if(cart.prev_turn == 1) // went straight before, so turn right
							cart.dir = direction::right, cart.x++;

						else if(cart.prev_turn == 2) // went right before, so turn left
							cart.dir = direction::left, cart.x--;

						break;

					case direction::down:
						if(cart.prev_turn == 0)      // went left before, so go straight
							cart.y++;

						else if(cart.prev_turn == 1) // went straight before, so turn right
							cart.dir = direction::left, cart.x--;

						else if(cart.prev_turn == 2) // went right before, so turn left
							cart.dir = direction::right, cart.x++;

						break;

					case direction::left:
						if(cart.prev_turn == 0)      // went left before, so go straight
							cart.x--;

						else if(cart.prev_turn == 1) // went straight before, so turn right
							cart.dir = direction::up, cart.y--;

						else if(cart.prev_turn == 2) // went right before, so turn left
							cart.dir = direction::down, cart.y++;

						break;

					case direction::right:
						if(cart.prev_turn == 0)      // went left before, so go straight
							cart.x++;

						else if(cart.prev_turn == 1) // went straight before, so turn right
							cart.dir = direction::down, cart.y++;

						else if(cart.prev_turn == 2) // went right before, so turn left
							cart.dir = direction::up, cart.y--;

						break;

					default:
						assert(!"!?!");
				}

				cart.prev_turn += 1;
				cart.prev_turn %= 3;
			}
			else
			{
				assert(!"?!?!");
			}


			// first remove ourselves from the previous tile
			track[{ px, py }] -= 10;

			// get the next tile
			int& next_tile = track[{ cart.x, cart.y }];
			if(next_tile >= 10)
			{
				// collision!
				// remove the offending carts (part 2)
				auto it = std::find_if(carts.begin(), carts.end(), [&cart](const auto& c) -> bool {
					return c.id != cart.id && c.x == cart.x && c.y == cart.y;
				});

				assert(it != carts.end() && "?!?!");

				cart.removed = true;
				it->removed = true;

				tfm::printfln("part 1: collision at t = %d, (%d, %d) (carts #%d and #%d)", tick, cart.x, cart.y,
					cart.id, it->id);

				// remove it.
				next_tile -= 10;
			}
			else
			{
				// set the next tile.
				next_tile += 10;
			}
		}

		// count the number of non-removed carts.
		{
			int non_removed = std::count_if(carts.begin(), carts.end(), [](const auto& c) -> bool {
				return !c.removed;
			});

			if(non_removed == 1)
			{
				// ok, now find it.
				auto it = std::find_if(carts.begin(), carts.end(), [](const auto& c) -> bool {
					return !c.removed;
				});

				assert(it != carts.end() && "?!?!!!!");

				tfm::printfln("part 2: last cart at (%d, %d)", it->x, it->y);

				quit = true;
				break;
			}
		}

		tick++;
	}
}








