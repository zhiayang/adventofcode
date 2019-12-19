// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"

struct path_t
{
	int dist;
	std::string doors;
};

static bool have_key(char door, const std::string& keys)
{
	return keys.find(tolower(door)) != -1;
}

static int search_1(std::map<v2, int>& map, v2 pos, std::map<char, v2>& keys, std::vector<char> remaining,
	const std::string& key_path, std::map<v2, path_t>& cache, int dist)
{
	if(remaining.empty())
	{
		fprintf(stderr, "\r                          \r%d: %s", dist, key_path.c_str());
		return dist;
	}

	int minimum_len = INT_MAX;

	// zpr::println("path: %s", key_path);

	for(char c : remaining)
	{
		auto path = cache[v2(key_path.back(), c)];

		bool have_all = true;
		for(auto d : path.doors)
		{
			if(!have_key(d, key_path))
			{
				// zpr::println("no key for %c (from %c) (%s.find(%c) == %d)", d, c, key_path, d, key_path.find(d));
				have_all = false;
				break;
			}
		}

		if(have_all)
		{
			auto copy = remaining;
			copy.erase(std::find(copy.begin(), copy.end(), c));

			auto len = search_1(map, keys[c], keys, copy, key_path + c, cache, dist + path.dist);
			minimum_len = std::min(len, minimum_len);
		}
	}

	return minimum_len;




	// for(char c : remaining)
	// {
	// 	auto dest = keys.find(c)->second;
	// 	// zpr::println("%s: %c: %c", dest, c, map[dest]);
	// 	auto path = util::astarSearch(pos, dest, [&](v2 pos) -> std::vector<v2> {
	// 		std::vector<v2> adj;
	// 		std::vector<v2> xs = { v2(0, 1), v2(0, -1), v2(1, 0), v2(-1, 0) };

	// 		for(auto x : xs)
	// 		{
	// 			auto p = pos + x;
	// 			if(map[p] == '.' || (map[p] >= 'a' && map[p] <= 'z'))
	// 			{
	// 				adj.push_back(p);
	// 			}
	// 			else if(map[p] >= 'A' && map[p] <= 'Z')
	// 			{
	// 				if(key_path.find(tolower(map[p]) != -1))
	// 				{
	// 					// zpr::println("have key for %c", map[p]);
	// 					adj.push_back(p);
	// 				}
	// 			}
	// 		}

	// 		return adj;

	// 	}, util::manhattan_dist);

	// 	if(!path.empty())
	// 	{
	// 		auto copy = remaining;
	// 		copy.erase(std::find(copy.begin(), copy.end(), c));

	// 		auto len = search_1(map, path.back(), copy, keys, key_path + c, cache);
	// 		minimum_len = std::min(len, minimum_len);
	// 	}
	// 	else
	// 	{
	// 		// zpr::println("* no path to %c", c);
	// 	}
	// }

	return minimum_len;
}

int main()
{
	auto s = util::readFile("input2.txt");

	std::map<v2, int> map;
	std::map<char, v2> keys;

	v2 start;
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
				map[pos] = c;

				if(c == '@')
					start = pos;

				if(c >= 'a' && c <= 'z')
					keys[c] = pos;

				pos += v2(1, 0);
			}
		}
	}

	char max_key = 0;
	for(char x = 'a'; x <= 'z'; x++)
		if(keys.find(x) != keys.end())
			max_key = x;


	std::map<v2, path_t> cache;
	for(char x = 'a'; x <= max_key; x++)
	{
		auto path = util::astarSearch(start, keys[x], [&](v2 pos) -> std::vector<v2> {
			std::vector<v2> adj;
			std::vector<v2> xs = { v2(0, 1), v2(0, -1), v2(1, 0), v2(-1, 0) };

			for(auto x : xs)
			{
				auto p = pos + x;
				if(map[p] != '#')
					adj.push_back(p);
			}

			return adj;

		}, util::manhattan_dist);

		if(!path.empty())
		{
			path_t p;
			p.dist = path.size() - 1;
			for(auto n : path)
			{
				if(map[n] >= 'A' && map[n] <= 'Z')
					p.doors += map[n];
			}

			cache[v2('@', x)] = p;
		}





		for(char y = 'a'; y <= max_key; y++)
		{
			if(x == y) continue;
			if(cache.find(v2(y, x)) != cache.end())
				continue;

			zpr::println("%c -> %c", x, y);
			auto path = util::astarSearch(keys[x], keys[y], [&](v2 pos) -> std::vector<v2> {
				std::vector<v2> adj;
				std::vector<v2> xs = { v2(0, 1), v2(0, -1), v2(1, 0), v2(-1, 0) };

				for(auto x : xs)
				{
					auto p = pos + x;
					if(map[p] != '#')
						adj.push_back(p);
				}

				return adj;

			}, util::manhattan_dist);

			if(!path.empty())
			{
				path_t p;
				p.dist = path.size() - 1;
				for(auto n : path)
				{
					if(map[n] >= 'A' && map[n] <= 'Z')
						p.doors += map[n];
				}

				cache[v2(x, y)] = p;
				cache[v2(y, x)] = p;
			}
		}
	}


	int x = search_1(map, start, keys, util::rangeClosed('a', max_key), "@", cache, 0);
	zpr::println("part 1: %d", x);
}




