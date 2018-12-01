// prog.cpp
// Copyright (c) 2014 - 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <set>
#include <map>
#include <deque>
#include <array>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include <algorithm>
#include <string_view>
#include <unordered_map>


std::vector<std::string> rotate2(std::vector<std::string> in)
{
	/*
		12
		34

		31
		42

		43
		21

		24
		13

		12
		34
	*/

	std::vector<std::string> ret(2);
	ret[0] = "xx";
	ret[1] = "xx";

	ret[0][0] = in[1][0];
	ret[0][1] = in[0][0];
	ret[1][0] = in[1][1];
	ret[1][1] = in[0][1];

	return ret;
};



std::vector<std::string> flipH2(std::vector<std::string> in)
{
	/*
		12
		34

		21
		43
	*/

	std::vector<std::string> ret(2);
	ret[0] = "xx";
	ret[1] = "xx";

	ret[0][0] = in[0][1];
	ret[0][1] = in[0][0];

	ret[1][0] = in[1][1];
	ret[1][1] = in[1][0];

	return ret;
};



std::vector<std::string> flipV2(std::vector<std::string> in)
{
	/*
		12
		34

		34
		12
	*/

	std::vector<std::string> ret(2);
	ret[0] = in[1];
	ret[1] = in[0];

	return ret;
};


std::vector<std::string> rotate3(std::vector<std::string> in)
{
	/*
		123
		456
		789

		741
		852
		963

		987
		654
		321

		369
		258
		147

		123
		456
		789
	*/

	std::vector<std::string> ret(3);
	ret[0] = "xxx";
	ret[1] = "xxx";
	ret[2] = "xxx";

	ret[0][0] = in[2][0];
	ret[0][1] = in[1][0];
	ret[0][2] = in[0][0];

	ret[1][0] = in[2][1];
	ret[1][1] = in[1][1];
	ret[1][2] = in[0][1];

	ret[2][0] = in[2][2];
	ret[2][1] = in[1][2];
	ret[2][2] = in[0][2];

	return ret;
};

std::vector<std::string> flipH3(std::vector<std::string> in)
{
	/*
		123
		456
		789

		321
		654
		987
	*/

	std::vector<std::string> ret(3);
	ret[0] = "xxx";
	ret[1] = "xxx";
	ret[2] = "xxx";

	ret[0][0] = in[0][2];
	ret[0][1] = in[0][1];
	ret[0][2] = in[0][0];

	ret[1][0] = in[1][2];
	ret[1][1] = in[1][1];
	ret[1][2] = in[1][0];

	ret[2][0] = in[2][2];
	ret[2][1] = in[2][1];
	ret[2][2] = in[2][0];

	return ret;
};



std::vector<std::string> flipV3(std::vector<std::string> in)
{
	/*
		123
		456
		789

		789
		456
		123
	*/

	std::vector<std::string> ret(3);
	ret[0] = in[2];
	ret[1] = in[1];
	ret[2] = in[0];

	return ret;
};


std::vector<std::string> subgrid2(const std::vector<std::string>& grid, int x, int y)
{
	// 1234
	// 5678
	// ABCD
	// EFGH

	auto l1 = grid[y].substr(x, 2);
	auto l2 = grid[y + 1].substr(x, 2);

	return { l1, l2 };
}


std::vector<std::string> subgrid3(const std::vector<std::string>& grid, int x, int y)
{
	// 012345
	// 6789AB
	// CDEFGH
	// IJKLMN
	// OPQRST
	// UVWXYZ

	auto l1 = grid[y].substr(x, 3);
	auto l2 = grid[y + 1].substr(x, 3);
	auto l3 = grid[y + 2].substr(x, 3);

	return { l1, l2, l3 };
}





int main()
{
	std::unordered_map<std::string, std::vector<std::string>> rules2;
	std::unordered_map<std::string, std::vector<std::string>> rules3;

	std::vector<std::string> input2 = {
		"../.. => ###/#../.#.",
		"#./.. => ##./.#./...",
		"##/.. => ..#/.#./#.#",
		".#/#. => ..#/.#./..#",
		"##/#. => .../.##/##.",
		"##/## => ###/#../#.."
	};

	for(auto r : input2)
	{
		std::vector<std::string> grid = { r.substr(0, 2), r.substr(3, 2) };

		std::vector<std::string> out = { r.substr(9, 3), r.substr(13, 3), r.substr(17, 3) };
 		rules2[grid[0] + grid[1]] = out;
 		rules2[flipH2(grid)[0] + flipH2(grid)[1]] = out;
 		rules2[flipV2(grid)[0] + flipV2(grid)[1]] = out;

		// rotate 3 times, and add the flipped ones too.
		for(int i = 0; i < 3; i++)
		{
			grid = rotate2(grid);

	 		rules2[grid[0] + grid[1]] = out;
	 		rules2[flipH2(grid)[0] + flipH2(grid)[1]] = out;
	 		rules2[flipV2(grid)[0] + flipV2(grid)[1]] = out;
		}
	}

	std::vector<std::string> input3 = {
		".../.../... => .#../.#../#..#/##..",
		"#../.../... => ####/####/.###/####",
		".#./.../... => ####/..../#.#./.#.#",
		"##./.../... => ..##/###./...#/##.#",
		"#.#/.../... => .#../#..#/.#../#.#.",
		"###/.../... => #.##/..##/##.#/..##",
		".#./#../... => .##./#..#/..../....",
		"##./#../... => ##../.#../...#/####",
		"..#/#../... => ##../###./...#/.#.#",
		"#.#/#../... => ####/#.../..../##..",
		".##/#../... => #..#/..##/#..#/....",
		"###/#../... => #.##/####/..#./#.#.",
		".../.#./... => #.##/.#.#/#.../...#",
		"#../.#./... => .###/##.#/..../###.",
		".#./.#./... => ..#./.#../..../##..",
		"##./.#./... => ##../...#/..../....",
		"#.#/.#./... => ####/.#../..#./.###",
		"###/.#./... => ..#./.###/##../.##.",
		".#./##./... => ###./#.#./.###/.##.",
		"##./##./... => ...#/.#../.#../####",
		"..#/##./... => ..#./#.../##../###.",
		"#.#/##./... => #.../..../.#.#/.###",
		".##/##./... => #.#./.#../####/.###",
		"###/##./... => .#.#/#.#./##../#...",
		".../#.#/... => #.##/##.#/..../#.#.",
		"#../#.#/... => ##../#.##/###./###.",
		".#./#.#/... => ##../.#../#.##/###.",
		"##./#.#/... => ##../##../..#./..#.",
		"#.#/#.#/... => #.../.##./.###/###.",
		"###/#.#/... => ##.#/##../.##./#...",
		".../###/... => ...#/####/..../#..#",
		"#../###/... => ##.#/##.#/.##./#.#.",
		".#./###/... => .#../#.../.#.#/##.#",
		"##./###/... => ##.#/#.#./#.../.#..",
		"#.#/###/... => ..../#.../####/.#..",
		"###/###/... => .#../#..#/.#../.#..",
		"..#/.../#.. => .#.#/#.../..##/...#",
		"#.#/.../#.. => ####/####/###./...#",
		".##/.../#.. => ####/.###/##.#/##..",
		"###/.../#.. => ..##/..../...#/#.#.",
		".##/#../#.. => ###./..#./##.#/##.#",
		"###/#../#.. => ##.#/...#/.##./.###",
		"..#/.#./#.. => #.#./#.#./...#/#.#.",
		"#.#/.#./#.. => ###./.#.#/#.#./.#..",
		".##/.#./#.. => #.#./.##./.###/#.#.",
		"###/.#./#.. => #.../#.../#.#./.###",
		".##/##./#.. => .#.#/.##./..#./##..",
		"###/##./#.. => .###/.##./#.##/..##",
		"#../..#/#.. => #.#./#..#/###./.##.",
		".#./..#/#.. => ###./.###/...#/..##",
		"##./..#/#.. => ###./##../####/.#.#",
		"#.#/..#/#.. => ..#./.#../.##./.#..",
		".##/..#/#.. => ##.#/###./.##./#...",
		"###/..#/#.. => ...#/..##/##.#/##.#",
		"#../#.#/#.. => #.../.##./.#.#/.###",
		".#./#.#/#.. => #.##/...#/####/###.",
		"##./#.#/#.. => .#../#.../.###/....",
		"..#/#.#/#.. => ####/###./.#.#/#...",
		"#.#/#.#/#.. => ###./..##/...#/#.##",
		".##/#.#/#.. => ##.#/..#./..##/.#.#",
		"###/#.#/#.. => #.#./..../##../.###",
		"#../.##/#.. => #..#/###./.#.#/##.#",
		".#./.##/#.. => #.../.###/.##./.###",
		"##./.##/#.. => .#../###./.#../##.#",
		"#.#/.##/#.. => .#../#.#./.#../#.##",
		".##/.##/#.. => ##../###./.#.#/.###",
		"###/.##/#.. => ..##/...#/#.../.#..",
		"#../###/#.. => #.##/#..#/####/###.",
		".#./###/#.. => .###/.#.#/#.#./..#.",
		"##./###/#.. => ####/#.#./..##/#.##",
		"..#/###/#.. => .###/##.#/.##./#.#.",
		"#.#/###/#.. => #.##/###./.###/....",
		".##/###/#.. => #.##/..../.#../####",
		"###/###/#.. => ##.#/###./.#../...#",
		".#./#.#/.#. => ..#./##.#/.#../###.",
		"##./#.#/.#. => ..##/###./..#./.#.#",
		"#.#/#.#/.#. => .#../..##/.#.#/.#.#",
		"###/#.#/.#. => ##../#..#/.#../..#.",
		".#./###/.#. => #.../#..#/.#.#/....",
		"##./###/.#. => ..../..##/..#./####",
		"#.#/###/.#. => ..##/##.#/.###/...#",
		"###/###/.#. => ##.#/#.##/..#./#.#.",
		"#.#/..#/##. => #.../####/#.##/.###",
		"###/..#/##. => ###./...#/.#.#/#..#",
		".##/#.#/##. => ..../.#.#/##.#/..##",
		"###/#.#/##. => ###./.#../.#.#/###.",
		"#.#/.##/##. => ###./.#../.#../.#.#",
		"###/.##/##. => .##./..../..../#.##",
		".##/###/##. => ####/##../.###/##.#",
		"###/###/##. => #..#/#.##/#.##/.#..",
		"#.#/.../#.# => ####/#.#./#..#/.##.",
		"###/.../#.# => .#../.#.#/.#../.#.#",
		"###/#../#.# => ..#./..#./.###/#...",
		"#.#/.#./#.# => #.#./..../.##./####",
		"###/.#./#.# => #.../..##/.##./..#.",
		"###/##./#.# => .#.#/##../#.#./..#.",
		"#.#/#.#/#.# => #.##/#.##/#.##/..##",
		"###/#.#/#.# => .###/#.#./.##./..##",
		"#.#/###/#.# => ...#/#.#./..#./#..#",
		"###/###/#.# => #.../#..#/#..#/.##.",
		"###/#.#/### => .#.#/..##/##.#/#...",
		"###/###/### => .###/#.#./#.../#..."
	};

	for(auto r : input3)
	{
		std::vector<std::string> grid = { r.substr(0, 3), r.substr(4, 3), r.substr(8, 3) };
		std::vector<std::string> out = { r.substr(15, 4), r.substr(20, 4), r.substr(25, 4), r.substr(30, 4) };

 		rules3[grid[0] + grid[1] + grid[2]] = out;
 		rules3[flipH3(grid)[0] + flipH3(grid)[1] + flipH3(grid)[2]] = out;
 		rules3[flipV3(grid)[0] + flipV3(grid)[1] + flipV3(grid)[2]] = out;

		// rotate 3 times, and add the flipped ones too.
		for(int i = 0; i < 3; i++)
		{
			grid = rotate3(grid);

	 		rules3[grid[0] + grid[1] + grid[2]] = out;
	 		rules3[flipH3(grid)[0] + flipH3(grid)[1] + flipH3(grid)[2]] = out;
	 		rules3[flipV3(grid)[0] + flipV3(grid)[1] + flipV3(grid)[2]] = out;
		}
	}

	using grid_t = std::vector<std::string>;
	grid_t grid = {
		".#.",
		"..#",
		"###"
	};

	for(auto l : grid) printf("%s\n", l.c_str());
	printf("\n");


	for(int i = 0; i < 18; i++)
	{
		int sz = 2;
		if(grid.size() % 2 != 0)
			sz = 3;

		// split the thing
		auto gridsz = grid.size() / sz;

		std::vector<std::vector<grid_t>> subs;
		subs.resize(gridsz);
		for(auto& g : subs) g.resize(gridsz);

		int total = grid.size() * grid.size();

		for(int j = 0; j < grid.size(); j += sz)
		{
			for(int k = 0; k < grid.size(); k += sz)
			{
				auto g = (sz == 2 ? subgrid2(grid, j, k) : subgrid3(grid, j, k));
				std::vector<std::string> out;

				out = (sz == 2 ? rules2[g[0] + g[1]] : rules3[g[0] + g[1] + g[2]]);

				subs[k / sz][j / sz] = out;

				fprintf(stderr, "\r    \r%.2f%%", 100.0 * ((double) (i + j * grid.size()) / (double) total));
			}
		}

		grid.clear();
		for(const auto& sub : subs)
		{
			for(int k = 0; k < sz + 1; k++)
			{
				std::string line;
				for(const auto& subsub : sub)
					line += subsub[k];

				grid.push_back(line);
			}
		}

		printf("\tit %d (%ld x %ld)\n", i, grid.size(), grid.size());
	}

	size_t lights = 0;
	for(auto l : grid)
		lights += std::count(l.begin(), l.end(), '#');

	printf("%ld lights on\n", lights);
}







