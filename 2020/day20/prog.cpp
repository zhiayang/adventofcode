// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

struct Tile
{
	int64_t id = 0;
	std::vector<std::string> data;
};

static constexpr size_t TILE_SIZE = 10;

std::vector<std::string> rotate90CW(const std::vector<std::string>& data)
{
	std::vector<std::string> new_data;

	auto dim = data.size();
	for(size_t x = 0; x < dim; x++)
	{
		std::string line;
		for(size_t y = 0; y < dim; y++)
			line.push_back(data[y][x]);

		std::reverse(line.begin(), line.end());
		new_data.push_back(std::move(line));
	}

	return new_data;
}
std::vector<std::string> flip_horz(std::vector<std::string> data)
{
	auto dim = data.size();
	for(size_t y = 0; y < dim; y++)
		std::reverse(data[y].begin(), data[y].end());

	return data;
}
std::vector<std::string> flip_vert(std::vector<std::string> data)
{
	std::reverse(data.begin(), data.end());
	return data;
}


Tile rotate90CW(Tile tile)
{
	return Tile {
		.id = tile.id,
		.data = rotate90CW(tile.data)
	};
}

Tile flip_horz(Tile tile)
{
	tile.data = flip_horz(tile.data);
	return tile;
}

Tile flip_vert(Tile tile)
{
	tile.data = flip_vert(tile.data);
	return tile;
}

// check if B fits on the right of A
bool match_right(const Tile& a, const Tile& b)
{
	// get the last column of A
	std::string ac;
	for(size_t y = 0; y < TILE_SIZE; y++)
		ac.push_back(a.data[y][9]);

	std::string bc;
	for(size_t y = 0; y < TILE_SIZE; y++)
		bc.push_back(b.data[y][0]);

	return ac == bc;
}

bool match_left(const Tile& a, const Tile& b)
{
	return match_right(b, a);
}

bool match_bottom(const Tile& a, const Tile& b)
{
	std::string ac = a.data.back();
	std::string bc = b.data.front();

	return ac == bc;
}

bool match_top(const Tile& a, const Tile& b)
{
	return match_bottom(b, a);
}

std::vector<std::string> remove_monsters(std::vector<std::string> image)
{
	std::vector<std::string> monster = {
		"                  # ",
		"#    ##    ##    ###",
		" #  #  #  #  #  #   "
	};

	auto match_line = [](str_view main, str_view sub) -> bool {
		if(main.size() < sub.size())
			return false;

		for(size_t si = 0; si < sub.size(); si++)
		{
			if(sub[si] == ' ')
				continue;

			else if(main[si] != sub[si])
				return false;
		}

		return true;
	};

	for(size_t y = 0; y < image.size() - 2; y++)
	{
		for(size_t x = 0; x < image[y].size(); x++)
		{
			auto l1 = str_view(image[y + 0]).drop(x);
			auto l2 = str_view(image[y + 1]).drop(x);
			auto l3 = str_view(image[y + 2]).drop(x);

			if(match_line(l1, monster[0]) && match_line(l2, monster[1]) && match_line(l3, monster[2]))
			{
				// replace the monster with 'O'
				for(size_t i = 0; i < monster[0].size(); i++)
				{
					if(monster[0][i] != ' ') image[y + 0][x + i] = 'O';
					if(monster[1][i] != ' ') image[y + 1][x + i] = 'O';
					if(monster[2][i] != ' ') image[y + 2][x + i] = 'O';
				}
			}
		}
	}

	return image;
}



int main()
{
	auto input = readFile("input.txt");
	auto parts = split(input, "\n\n");

	std::map<int, Tile> tiles;
	std::map<v2, Tile> positions;

	std::map<int, bool> fitted;

	for(auto& tile : parts)
	{
		auto lines = split(tile, "\n");
		auto id = to_int(lines[0].drop(5).drop_last(1));

		std::vector<std::string> data;
		for(size_t i = 1; i < lines.size(); i++)
			data.push_back(lines[i].str());

		tiles[id] = Tile {
			.id = id,
			.data = data
		};
	}

	{
		std::deque<std::pair<Tile, v2>> queue;

		queue.push_back({ tiles.begin()->second, v2(0, 0) });
		positions[v2(0, 0)] = tiles.begin()->second;

		while(!queue.empty())
		{
			auto [ tile, pos ] = queue.front();
			queue.pop_front();

			auto id = tile.id;

			for(auto& [ id1, tile1 ] : tiles)
			{
				if(id == id1 || fitted[id1])
					continue;

				std::vector<Tile> orients2 = {
					tile1,
					flip_vert(tile1),
					flip_horz(tile1),
					rotate90CW(tile1),
					rotate90CW(rotate90CW(tile1)),
					rotate90CW(rotate90CW(rotate90CW(tile1))),
					rotate90CW(flip_vert(tile1)),
					rotate90CW(rotate90CW(flip_vert(tile1))),
					rotate90CW(rotate90CW(rotate90CW(flip_vert(tile1)))),
				};

				for(auto& t2 : orients2)
				{
					if(match_top(tile, t2))
					{
						fitted[tile.id] = true;
						fitted[tile1.id] = true;

						auto npos = pos + v2(0, -1);
						positions[npos] = t2;
						queue.push_back({ t2, npos });
						break;
					}
					else if(match_bottom(tile, t2))
					{
						fitted[tile.id] = true;
						fitted[tile1.id] = true;

						auto npos = pos + v2(0, +1);
						positions[npos] = t2;
						queue.push_back({ t2, npos });
						break;
					}
					else if(match_left(tile, t2))
					{
						fitted[tile.id] = true;
						fitted[tile1.id] = true;

						auto npos = pos + v2(-1, 0);
						positions[npos] = t2;
						queue.push_back({ t2, npos });
						break;
					}
					else if(match_right(tile, t2))
					{
						fitted[tile.id] = true;
						fitted[tile1.id] = true;

						auto npos = pos + v2(+1, 0);
						positions[npos] = t2;
						queue.push_back({ t2, npos });
						break;
					}
				}
			}
		}
	}

	// now to find the corners...
	v2 UL = {};
	v2 UR = {};
	v2 BL = {};
	v2 BR = {};

	{
		auto top = [&](v2 start) -> v2 {
			auto pos = start;
			while(positions.find(pos + v2(0, -1)) != positions.end())
				pos += v2(0, -1);

			return pos;
		};

		auto bottom = [&](v2 start) -> v2 {
			auto pos = start;
			while(positions.find(pos + v2(0, +1)) != positions.end())
				pos += v2(0, +1);

			return pos;
		};

		auto left = [&](v2 start) -> v2 {
			auto pos = start;
			while(positions.find(pos + v2(-1, 0)) != positions.end())
				pos += v2(-1, 0);

			return pos;
		};

		auto right = [&](v2 start) -> v2 {
			auto pos = start;
			while(positions.find(pos + v2(+1, 0)) != positions.end())
				pos += v2(+1, 0);

			return pos;
		};

		UL = top(left(v2(0, 0)));
		UR = top(right(v2(0, 0)));
		BL = bottom(left(v2(0, 0)));
		BR = bottom(right(v2(0, 0)));
	}

	zpr::println("part 1 = {}", positions[UL].id * positions[UR].id * positions[BL].id * positions[BR].id);

	// now, assemble the map...
	{
		std::vector<std::string> image;

		// construct it row by row, starting at the upper left corner:
		auto len = UR.x - UL.x + 1;
		assert(BL.y - UL.y + 1 == len);

		for(size_t y = 0; y < len; y++)
		{
			size_t yofs = (TILE_SIZE - 2) * y;
			for(size_t i = 0; i < TILE_SIZE - 2; i++)
				image.push_back("");

			for(size_t x = 0; x < len; x++)
			{
				auto pos = UL + v2(x, y);
				auto& tile = positions[pos];

				for(size_t r = 0; r < TILE_SIZE - 2; r++)
				{
					str_view sv = tile.data[1 + r];
					sv = sv.drop(1).drop_last(1);
					image[yofs + r] += sv.str();
				}
			}
		}

		auto dim = image[0].size();
		std::vector<std::vector<std::string>> orients = {
			image,
			flip_horz(image),
			flip_vert(image),
			rotate90CW(image),
			rotate90CW(rotate90CW(image)),
			rotate90CW(rotate90CW(rotate90CW(image))),
			rotate90CW(flip_vert(image)),
			rotate90CW(rotate90CW(flip_vert(image))),
			rotate90CW(rotate90CW(rotate90CW(flip_vert(image))))
		};

		for(auto& img : orients)
		{
			auto copy = remove_monsters(img);
			if(copy != img)
			{
				// count the '#s'
				size_t n = 0;
				for(auto& l : copy)
					for(auto c : l)
						if(c == '#')
							n++;

				zpr::println("part 2 = {}", n);
			}
		}
	}
}

