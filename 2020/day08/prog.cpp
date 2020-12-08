// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

static std::optional<int64_t> run(const std::vector<str_view>& lines, size_t flipK, bool flip)
{
	int64_t accum = 0;
	std::unordered_map<size_t, bool> seen;

	size_t ip = 0;
	while(ip < lines.size())
	{
		if(seen[ip])
		{
			if(flip) return { };
			else     return accum;
		}

		seen[ip] = true;
		auto& instr = lines[ip];

		if(instr.find("acc") == 0)
		{
			accum += to_int(instr.drop(4));
			ip++;
		}
		else if(instr.find("nop") == 0)
		{
			if(flip && flipK == ip)
				goto jmp;

		nop:
			ip++;
		}
		else if(instr.find("jmp") == 0)
		{
			if(flip && flipK == ip)
				goto nop;

		jmp:
			ip += to_int(instr.drop(4));
		}
	}

	return accum;
}

int main()
{
	auto lines = readFileLines("input.txt", '\n');

	zpr::println("part 1 = {}", *run(lines, 0, false));

	for(size_t k = 0; k < lines.size(); k++)
	{
		if(auto acc = run(lines, k, true); acc)
		{
			zpr::println("part 2 = {}", *acc);
			break;
		}
	}
}









