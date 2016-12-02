// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 18

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <map>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

FILE* openInput(size_t* size, uint8_t** buf)
{
	FILE* f = fopen("input", "r");
	if(f == 0)
	{
		fprintf(stderr, "file read error FILE* was null :: %d / %s\n", errno, strerror(errno));
		exit(-1);
	}

	struct stat st;
	fstat(fileno(f), &st);

	*size = st.st_size;
	*buf = new uint8_t[*size + 1];
	if(*buf == 0)
	{
		fprintf(stderr, "failed to allocate %zu bytes\n", *size + 1);
		exit(-1);
	}

	size_t readbytes = fread(*buf, 1, *size, f);
	if(readbytes != *size)
	{
		fprintf(stderr, "file read error (need %zu bytes, read %zu) :: %d / %s\n", *size, readbytes, errno, strerror(errno));
		exit(-1);
	}

	return f;
}



enum class Pos
{
	TOP,
	TOP_RIGHT,
	RIGHT,
	BOTTOM_RIGHT,
	BOTTOM,
	BOTTOM_LEFT,
	LEFT,
	TOP_LEFT,
};

#define SIZE 100

int ind(int x, int y)
{
	return y * SIZE + x;
}

bool getNeighbourState(std::array<bool, SIZE * SIZE>& grid, int x, int y, Pos p)
{
	int nx = 0;
	int ny = 0;

	// 0, 0 is top-left.

	switch(p)
	{
		case Pos::TOP:
			nx = x;
			ny = y - 1;
			break;

		case Pos::TOP_RIGHT:
			nx = x + 1;
			ny = y - 1;
			break;

		case Pos::RIGHT:
			nx = x + 1;
			ny = y;
			break;

		case Pos::BOTTOM_RIGHT:
			nx = x + 1;
			ny = y + 1;
			break;

		case Pos::BOTTOM:
			nx = x;
			ny = y + 1;
			break;

		case Pos::BOTTOM_LEFT:
			nx = x - 1;
			ny = y + 1;
			break;

		case Pos::LEFT:
			nx = x - 1;
			ny = y;
			break;

		case Pos::TOP_LEFT:
			nx = x - 1;
			ny = y - 1;
			break;
	}

	if(nx < 0 || ny < 0 || nx >= SIZE || ny >= SIZE)
		return 0;

	return grid[ind(nx, ny)];
}

int getOnNeighbours(std::array<bool, SIZE * SIZE>& grid, int x, int y)
{
	int count = 0;
	if(getNeighbourState(grid, x, y, Pos::TOP)) count++;
	if(getNeighbourState(grid, x, y, Pos::TOP_RIGHT)) count++;
	if(getNeighbourState(grid, x, y, Pos::RIGHT)) count++;
	if(getNeighbourState(grid, x, y, Pos::BOTTOM_RIGHT)) count++;
	if(getNeighbourState(grid, x, y, Pos::BOTTOM)) count++;
	if(getNeighbourState(grid, x, y, Pos::BOTTOM_LEFT)) count++;
	if(getNeighbourState(grid, x, y, Pos::LEFT)) count++;
	if(getNeighbourState(grid, x, y, Pos::TOP_LEFT)) count++;

	return count;
}

void update(std::array<bool, SIZE * SIZE>* grid)
{
	std::array<bool, SIZE * SIZE> newg;

	for(int y = 0; y < SIZE; y++)
	{
		for(int x = 0; x < SIZE; x++)
		{
			int n = getOnNeighbours(*grid, x, y);

			if((*grid)[ind(x, y)])	// light is on
			{
				// stays on if 2 or 3 neighbours on
				if(n != 2 && n != 3)
				{
					// disable check for part 1.
					// in part 2, corner lights are stuck on.
					if(!((x == 0 && y == 0) || (x == 0 && y == SIZE - 1) || (x == SIZE - 1 && y == 0) || (x == SIZE - 1 && y == SIZE - 1)))
						newg[ind(x, y)] = 0;

					else
						newg[ind(x, y)] = 1;
				}
				else
				{
					newg[ind(x, y)] = 1;
				}
			}
			else				// light is off
			{
				// turns on if 3 neighbours on
				if(n == 3)
				{
					newg[ind(x, y)] = 1;
				}
				else
				{
					// see above
					if(!((x == 0 && y == 0) || (x == 0 && y == SIZE - 1) || (x == SIZE - 1 && y == 0) || (x == SIZE - 1 && y == SIZE - 1)))
						newg[ind(x, y)] = 0;

					else
						newg[ind(x, y)] = 1;
				}
			}
		}
	}

	*grid = newg;
}








int main()
{
	size_t size = 0;
	uint8_t* buf = 0;

	FILE* f = openInput(&size, &buf);


	std::vector<std::string> strings;
	std::string cur = "";

	for(size_t i = 0; i < size; i++)
	{
		if(buf[i] != '\n' && buf[i] != 0)
		{
			cur += buf[i];
		}
		else
		{
			strings.push_back(cur);
			cur = "";
		}
	}




	std::array<bool, SIZE * SIZE> grid;

	size_t x = 0;
	size_t y = 0;

	for(auto line : strings)
	{
		for(auto light : line)
		{
			if(light == '.')
				grid[ind(x, y)] = 0;

			else
				grid[ind(x, y)] = 1;

			x++;
		}

		x = 0;

		y++;
	}


	// stick the corner lights (in case input is ... iffy)
	grid[ind(0, 0)] = 1;
	grid[ind(0, SIZE - 1)] = 1;
	grid[ind(SIZE - 1, 0)] = 1;
	grid[ind(SIZE - 1, SIZE - 1)] = 1;


	for(int i = 0; i < 100; i++)
		update(&grid);

	// for(size_t y = 0; y < SIZE; y++)
	// {
	// 	for(size_t x = 0; x < SIZE; x++)
	// 		printf("%c", grid[ind(x, y)] ? '*' : '-');

	// 	printf("\n");
	// }



	int count = 0;
	for(auto l : grid)
		if(l) count++;

	printf("%d lights on\n", count);





	fclose(f);
}












