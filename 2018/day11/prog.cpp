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

int power(int grid, int x, int y)
{
	// we give 0-coords but the power-level does 1-coords
	x += 1;
	y += 1;

	int rack_id = x + 10;
	int pwr = rack_id * y;
	pwr += grid;
	pwr *= rack_id;

	if(pwr < 100)
		pwr = 0;

	else
		pwr = (pwr / 100) % 10;

	return pwr - 5;
}

int power(int* grid, int x, int y)
{
	return grid[x + y * 300];
}

static std::map<std::tuple<int, int, int>, int> cache;
int check(int* grid, int x, int y, int s)
{
	if(x < 301 - s && y < 301 - s)
	{
		int pwr = 0;
		if(s > 3)
		{
			pwr = cache[{ x, y, s - 1 }];

			// calc the extra on the sides.
			for(int i = 0; i < s; i++)
				pwr += power(grid, x + i, y + s - 1);

			for(int j = 0; j < s; j++)
				pwr += power(grid, x + s - 1, y + j);

			// and the corner
			pwr += power(grid, x + s - 1, y + s - 1);
		}
		else
		{
			for(int i = 0; i < s; i++)
			{
				for(int j = 0; j < s; j++)
				{
					pwr += power(grid, x + i, y + j);
				}
			}
		}

		cache[{ x, y, s }] = pwr;
		return pwr;
	}
	else
	{
		return 0;
	}
}

int main()
{
	int grid_id = 7672;

	int* grid = new int[300 * 300];
	for(int x = 0; x < 300; x++)
	{
		for(int y = 0; y < 300; y++)
		{
			grid[x + y * 300] = power(grid_id, x, y);
		}
	}



	{
		// check each grid piece.
		int x_of_max = 0;
		int y_of_max = 0;
		int max_pwr = 0;

		for(int x = 0; x < 300; x++)
		{
			for(int y = 0; y < 300; y++)
			{
				int p = check(grid, x, y, 3);
				if(p > max_pwr)
				{
					x_of_max = x;
					y_of_max = y;
					max_pwr = p;
				}
			}
		}

		tfm::printfln("part 1: max power of %d with coords (%d, %d)", max_pwr, x_of_max + 1, y_of_max + 1);
	}


	{
		// check each grid piece.
		int x_of_max = 0;
		int y_of_max = 0;
		int s_of_max = 0;
		int max_pwr = 0;

		for(int x = 0; x < 300; x++)
		{
			for(int y = 0; y < 300; y++)
			{
				for(int s = 1; s <= 300; s++)
				{
					fprintf(stderr, "\r                             \rchecking: (%d, %d) x %d", x, y, s);
					int p = check(grid, x, y, s);
					if(p > max_pwr)
					{
						x_of_max = x;
						y_of_max = y;
						s_of_max = s;
						max_pwr = p;
					}
				}
			}
		}

		tfm::printfln("\n\npart 2: max power of %d with coords (%d, %d) and size %d", max_pwr, x_of_max + 1, y_of_max + 1, s_of_max);
	}
}
















