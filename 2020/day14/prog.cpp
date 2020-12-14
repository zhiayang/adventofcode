// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

static void set(std::unordered_map<uint64_t, uint64_t>& mem, uint64_t addr, uint64_t val, const std::string& mask, int bit)
{
	if(bit == 0)
	{
		mem[addr] = val;
		// zpr::println("[{}] = {}", addr, val);
		return;
	}

	auto b = mask[bit - 1];
	if(b == '0')
	{
		set(mem, addr, val, mask, bit - 1);
	}
	else if(b == '1')
	{
		set(mem, addr | (1ULL << (36 - bit)), val, mask, bit - 1);
	}
	else if(b == 'X')
	{
		set(mem, addr & ~(1ULL << (36 - bit)), val, mask, bit - 1);  // 0
		set(mem, addr |  (1ULL << (36 - bit)), val, mask, bit - 1);  // 1
	}
}

static uint64_t sum(const std::unordered_map<uint64_t, uint64_t>& mem)
{
	uint64_t ret = 0;
	for(const auto& [ _, x ] : mem)
		ret += x;

	return ret;
}

int main()
{
	auto lines = readFileLines("input.txt");
	{
		std::unordered_map<uint64_t, uint64_t> memory;

		uint64_t am = 0;
		uint64_t om = 0;

		for(auto& line : lines)
		{
			if(line.find("mask") == 0)
			{
				auto mask = line.drop(7);

				am = 0;
				om = 0;
				for(char c : mask)
				{
					am = (am << 1) | (c == '0' ? 0 : 1);
					om = (om << 1) | (c == '1' ? 1 : 0);
				}
			}
			else
			{
				auto addr = line.take(line.find("]")).drop(4);
				auto val = line.drop(line.find("=") + 2).trim();

				memory[to_int(addr)] = ((uint64_t) to_int(val) & am) | om;
			}
		}

		zpr::println("part 1 = {}", sum(memory));
	}

	{
		std::unordered_map<uint64_t, uint64_t> memory;

		std::string mask;
		for(auto& line : lines)
		{
			if(line.find("mask") == 0)
			{
				mask = line.drop(7).str();
			}
			else
			{
				auto addr = line.take(line.find("]")).drop(4);
				auto val = line.drop(line.find("=") + 2).trim();

				set(memory, to_int(addr), to_int(val), mask, 36);
			}
		}

		zpr::println("part 2 = {}", sum(memory));
	}
}
