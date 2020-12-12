// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

int main()
{
	auto lines = readFileLines("input.txt");

	{
		int heading = 90;
		auto pos = v2(0, 0);

		for(auto& line : lines)
		{
			char action = line[0];
			int num = to_int(line.drop(1));

			if(action == 'L')   heading -= num;
			if(action == 'R')   heading += num;
			if(action == 'E')   pos.x += num;
			if(action == 'W')   pos.x -= num;
			if(action == 'N')   pos.y += num;
			if(action == 'S')   pos.y -= num;
			if(action == 'F')
			{
				if(heading == 0)   pos.y += num;
				if(heading == 90)  pos.x += num;
				if(heading == 180) pos.y -= num;
				if(heading == 270) pos.x -= num;
			}

			while(heading < 0)    heading += 360;
			while(heading >= 360) heading -= 360;
		}

		zpr::println("part 1 = {}", abs(pos.x) + abs(pos.y));
	}

	{
		auto pos = v2(0, 0);
		auto waypoint = v2(10, 1);

		for(auto& line : lines)
		{
			char action = line[0];
			int num = to_int(line.drop(1));

			if(action == 'E')   waypoint.x += num;
			if(action == 'W')   waypoint.x -= num;
			if(action == 'N')   waypoint.y += num;
			if(action == 'S')   waypoint.y -= num;
			if(action == 'F')   pos += v2(num * waypoint.x, num * waypoint.y);
			if(action == 'R')
			{
				num = (360 - num);
				goto rotate;
			}
			if(action == 'L')
			{
			rotate:
				num /= 90;
				while(num > 0)
				{
					std::swap(waypoint.x, waypoint.y);
					waypoint.x *= -1;
					num--;
				}
			}
		}

		zpr::println("part 2 = {}", abs(pos.x) + abs(pos.y));
	}
}
