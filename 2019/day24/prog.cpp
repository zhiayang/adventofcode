// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"
#include "assert.h"


int main()
{
	auto s = util::readFile("input.txt");

	std::set<std::map<v2, int>> seen;
	std::map<v2, int> _grid;

	{
		v2 pos;
		for(char c : s)
		{
			if(c == '\n')
			{
				pos.x = 0;
				pos.y++;
			}
			else
			{
				_grid[pos] = (c == '#' ? 1 : 0);
				pos += v2(1, 0);
			}
		}
	}

	{
		auto grid = _grid;
		seen.insert(grid);
		while(true)
		{
			auto g = grid;

			for(int x = 0; x < 5; x++)
			{
				for(int y = 0; y < 5; y++)
				{
					auto v = v2(x, y);
					auto bugs = grid[v.up()] + grid[v.down()] + grid[v.left()] + grid[v.right()];

					if(grid[v] == 1)
					{
						if(bugs != 1)
							g[v] = 0;

						else
							g[v] = 1;
					}
					else
					{
						if(bugs == 1 || bugs == 2)
							g[v] = 1;

						else
							g[v] = 0;
					}
				}
			}

			grid = g;
			if(seen.find(grid) != seen.end())
			{
				int bio = 0;

				// calculate
				int pow = 1;
				for(int y = 0; y < 5; y++)
				{
					for(int x = 0; x < 5; x++)
					{
						auto v = v2(x, y);
						if(grid[v] == 1)
							bio += pow;


						pow *= 2;
					}
				}

				zpr::println("part 1: %d", bio);
				break;
			}

			seen.insert(grid);
		}
	}


	{
		std::map<int, std::map<v2, int>> grids;
		grids[0] = _grid;

		int minlev = 0;
		int maxlev = 0;

		for(int i = 0; i < 200; i++)
		{
			minlev -= 1;
			maxlev += 1;

			auto gs = grids;

			auto get_other_grid = [&grids](int level, v2 v) -> int {
				if(grids.find(level) == grids.end())
					return 0;

				return grids[level][v];
			};

			for(int level = minlev; level <= maxlev; level++)
			{
				auto g = grids[level];

				for(int x = 0; x < 5; x++)
				{
					for(int y = 0; y < 5; y++)
					{
						auto grid = grids[level];

						if(x == 2 && y == 2)
							continue;

						auto v = v2(x, y);

						auto up = [&grids, &get_other_grid](v2 v, int level) -> int {
							int bugs = 0;
							if(v.up() == v2(2, 2))
							{
								for(int i = 0; i < 5; i++)
									bugs += get_other_grid(level + 1, v2(i, 4));
							}
							else if(v.y == 0)
							{
								bugs += get_other_grid(level - 1, v2(2, 1));
							}
							else
							{
								bugs += get_other_grid(level, v.up());
							}

							return bugs;
						};

						auto down = [&grids, &get_other_grid](v2 v, int level) -> int {
							int bugs = 0;
							if(v.down() == v2(2, 2))
							{
								for(int i = 0; i < 5; i++)
									bugs += get_other_grid(level + 1, v2(i, 0));
							}
							else if(v.y == 4)
							{
								bugs += get_other_grid(level - 1, v2(2, 3));
							}
							else
							{
								bugs += get_other_grid(level, v.down());
							}

							return bugs;
						};

						auto left = [&grids, &get_other_grid](v2 v, int level) -> int {
							int bugs = 0;
							if(v.left() == v2(2, 2))
							{
								for(int i = 0; i < 5; i++)
									bugs += get_other_grid(level + 1, v2(4, i));
							}
							else if(v.x == 0)
							{
								bugs += get_other_grid(level - 1, v2(1, 2));
							}
							else
							{
								bugs += get_other_grid(level, v.left());
							}

							return bugs;
						};

						auto right = [&grids, &get_other_grid](v2 v, int level) -> int {
							int bugs = 0;
							if(v.right() == v2(2, 2))
							{
								for(int i = 0; i < 5; i++)
									bugs += get_other_grid(level + 1, v2(0, i));
							}
							else if(v.x == 4)
							{
								bugs += get_other_grid(level - 1, v2(3, 2));
							}
							else
							{
								bugs += get_other_grid(level, v.right());
							}

							return bugs;
						};

						int bugs = up(v, level) + down(v, level) + left(v, level) + right(v, level);

						if(grid[v] == 1)
						{
							if(bugs != 1)
								g[v] = 0;

							else
								g[v] = 1;
						}
						else
						{
							if(bugs == 1 || bugs == 2)
								g[v] = 1;

							else
								g[v] = 0;
						}
					}
				}

				gs[level] = g;
			}

			grids = gs;
		}


		int bugs = 0;
		for(int k = minlev; k <= maxlev; k++)
			bugs += util::sum(util::map(util::pairs(grids[k]), [](auto p) -> auto { return p.second; }));

		zpr::println("part 2: %d", bugs);
	}
}












