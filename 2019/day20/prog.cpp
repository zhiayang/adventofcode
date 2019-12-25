// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"

int main()
{
	auto s = util::readFile("input.txt");


	std::map<v2, std::string> map;
	std::map<std::string, std::pair<v2, v2>> portals;

	{
		std::map<v2, char> m;

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
				m[pos] = c;
				pos += v2(1, 0);
			}
		}

		// fix the thing.
		for(const auto& [ pos, c ] : m)
		{
			if(isalpha(m[pos]))
			{
				v2 portpos;
				std::string port;

				if(isalpha(m[pos.up()]))            port.append({ m[pos.up()], m[pos] });
				else if(isalpha(m[pos.down()]))     port.append({ m[pos], m[pos.down()] });
				else if(isalpha(m[pos.left()]))     port.append({ m[pos.left()], m[pos] });
				else if(isalpha(m[pos.right()]))    port.append({ m[pos], m[pos.right()] });

				// get the empty tile position.
				if(isalpha(m[pos.left()]) && m[pos.right()] == '.')
					portpos = pos;

				else if(isalpha(m[pos.right()]) && m[pos.left()] == '.')
					portpos = pos;

				else if(isalpha(m[pos.up()]) && m[pos.down()] == '.')
					portpos = pos;

				else if(isalpha(m[pos.down()]) && m[pos.up()] == '.')
					portpos = pos;

				else
					port = "";

				// we'll check each portal twice, but that's fine.
				if(!port.empty())
				{
					if(portals[port].first == v2()) portals[port].first = portpos;
					else                            portals[port].second = portpos;

					map[portpos] = port;
				}
			}
			else
			{
				map[pos] = std::string(1, c);
			}
		}
	}

	auto get_open = [&map](const v2& x) -> v2 {
		if(map[x.up()] == ".")      return x.up();
		if(map[x.down()] == ".")    return x.down();
		if(map[x.left()] == ".")    return x.left();
		if(map[x.right()] == ".")   return x.right();

		return x;
	};


	{
		auto start = get_open(portals["AA"].first);
		auto goal = get_open(portals["ZZ"].first);

		auto path = util::astarSearch(start, goal, [&map, &portals, &get_open](const v2& pos) -> std::vector<v2> {
			std::vector<v2> adj;

			if(map[pos.up()] == ".")    adj.push_back(pos.up());
			if(map[pos.down()] == ".")  adj.push_back(pos.down());
			if(map[pos.left()] == ".")  adj.push_back(pos.left());
			if(map[pos.right()] == ".") adj.push_back(pos.right());

			auto add = [&map, &portals, &adj, &get_open](const v2& p) {

				auto [ a, b ] = portals[map[p]];
				auto out = (a == p ? b : a);

				adj.push_back(get_open(out));
			};

			if(isalpha(map[pos.up()][0]) && map[pos.up()] != "AA" && map[pos.up()] != "ZZ")
				add(pos.up());

			if(isalpha(map[pos.down()][0]) && map[pos.down()] != "AA" && map[pos.down()] != "ZZ")
				add(pos.down());

			if(isalpha(map[pos.left()][0]) && map[pos.left()] != "AA" && map[pos.left()] != "ZZ")
				add(pos.left());

			if(isalpha(map[pos.right()][0]) && map[pos.right()] != "AA" && map[pos.right()] != "ZZ")
				add(pos.right());


			return adj;

		}, [](const v2& a, const v2& b) -> int { return 0; });

		zpr::println("part 1: %d", path.size() - 1);
	}





	{
		auto start = v3(get_open(portals["AA"].first), 0);
		auto goal = v3(get_open(portals["ZZ"].first), 0);

		auto path = util::astarSearch(start, goal, [&map, &portals, &get_open](const v3& _pos) -> std::vector<v3> {

			std::vector<v3> adj;

			auto z = _pos.z;
			auto pos = _pos.xy();

			if(map[pos.up()] == ".")    adj.push_back(v3(pos.up(), z));
			if(map[pos.down()] == ".")  adj.push_back(v3(pos.down(), z));
			if(map[pos.left()] == ".")  adj.push_back(v3(pos.left(), z));
			if(map[pos.right()] == ".") adj.push_back(v3(pos.right(), z));

			v3 out;
			std::string name;

			auto add = [&map, &portals, &z, &adj, &get_open](const v2& p, const v2& pp, int dir) {

				auto get_z_ofs = [&map, &dir](const v2& p) -> int {

					int z = (map[(
						dir == 0 ? p.up() :
						dir == 1 ? p.down() :
						dir == 2 ? p.left() :
						p.right()
					)] == " " ? 1 : -1);

					return z;
				};

				auto zo = get_z_ofs(pp);

				if(z + zo >= 0)
				{
					auto [ a, b ] = portals[map[p]];
					auto out = v3((a == p ? b : a), z + zo);

					adj.push_back(v3(get_open(out.xy()), out.z));
				}
			};

			if(isalpha(map[pos.up()][0]) && map[pos.up()] != "AA" && map[pos.up()] != "ZZ")
				add(pos.up(), pos.up().up(), 0);

			if(isalpha(map[pos.down()][0]) && map[pos.down()] != "AA" && map[pos.down()] != "ZZ")
				add(pos.down(), pos.down().down(), 1);

			if(isalpha(map[pos.left()][0]) && map[pos.left()] != "AA" && map[pos.left()] != "ZZ")
				add(pos.left(), pos.left().left(), 2);

			if(isalpha(map[pos.right()][0]) && map[pos.right()] != "AA" && map[pos.right()] != "ZZ")
				add(pos.right(), pos.right().right(), 3);

			return adj;

		}, [](const v3& a, const v3& b) -> int { return 0; });

		zpr::println("part 2: %d", path.size() - 1);
	}
}








