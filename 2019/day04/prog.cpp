// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"

static bool adj(const char* pw)
{
	return pw[0] == pw[1] || pw[1] == pw[2]
		|| pw[2] == pw[3] || pw[3] == pw[4]
		|| pw[4] == pw[5];
}

static bool incr(const char* pw)
{
	return pw[0] <= pw[1] && pw[1] <= pw[2]
		&& pw[2] <= pw[3] && pw[3] <= pw[4]
		&& pw[4] <= pw[5];
}

static bool checkpw(int pw)
{
	auto s = std::to_string(pw);
	return adj(s.c_str()) && incr(s.c_str());
}

static bool adj2(const char* pw)
{
	auto find2 = [&pw](char c) {
		auto next = [&pw](char c, int i) -> bool {
			if(i == 5) return false;

			return pw[i] == c && pw[i+1] == c;
		};

		for(int i = 0; i < 5; i++)
			if(next(c, i)) return true;

		return false;
	};

	auto find3 = [&pw](char c) {
		auto next = [&pw](char c, int i) -> bool {
			if(i >= 4) return false;

			return pw[i] == c && pw[i+1] == c && pw[i+2] == c;
		};

		for(int i = 0; i < 4; i++)
			if(next(c, i)) return true;

		return false;
	};


	for(char i = '0'; i <= '9'; i++)
	{
		if(find2(i) && !find3(i))
			return true;
	}

	return false;
}

static bool checkpw2(int pw)
{
	auto s = std::to_string(pw);
	return adj2(s.c_str()) && incr(s.c_str());
}


int main()
{
	int begin = 382345;
	int end   = 843167;

	{
		int valids = 0;
		for(int i = begin; i <= end; i++)
		{
			if(checkpw(i))
				valids++;
		}

		zpr::println("part 1: %d", valids);
	}


	{
		int valids = 0;
		for(int i = begin; i <= end; i++)
		{
			if(checkpw2(i))
				valids++;
		}

		zpr::println("part 2: %d", valids);
	}
}








