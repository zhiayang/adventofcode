// prog.cpp
// Copyright (c) 2018, zhiayang
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


using registers_t = std::array<int, 4>;



registers_t compute(const registers_t& before, int real_opcode, int a, int b, int c)
{
	registers_t out = before;

	/* addr */      if(real_opcode == 0)        out[c] = out[a] + out[b];
	/* addi */      else if(real_opcode == 1)   out[c] = out[a] + b;
	/* mulr */      else if(real_opcode == 2)   out[c] = out[a] * out[b];
	/* muli */      else if(real_opcode == 3)   out[c] = out[a] * b;
	/* andr */      else if(real_opcode == 4)   out[c] = out[a] & out[b];
	/* andi */      else if(real_opcode == 5)   out[c] = out[a] & b;
	/* orr */       else if(real_opcode == 6)   out[c] = out[a] | out[b];
	/* ori */       else if(real_opcode == 7)   out[c] = out[a] | b;
	/* setr */      else if(real_opcode == 8)   out[c] = out[a];
	/* seti */      else if(real_opcode == 9)   out[c] = a;
	/* ge (i/r) */  else if(real_opcode == 10)  (a > out[b]) ? out[c] = 1 : out[c] = 0;
	/* ge (r/i) */  else if(real_opcode == 11)  (out[a] > b) ? out[c] = 1 : out[c] = 0;
	/* ge (r/r) */  else if(real_opcode == 12)  (out[a] > out[b]) ? out[c] = 1 : out[c] = 0;
	/* eq (i/r) */  else if(real_opcode == 13)  (a == out[b]) ? out[c] = 1 : out[c] = 0;
	/* eq (r/i) */  else if(real_opcode == 14)  (out[a] == b) ? out[c] = 1 : out[c] = 0;
	/* eq (r/r) */  else if(real_opcode == 15)  (out[a] == out[b]) ? out[c] = 1 : out[c] = 0;
	else                                        assert(false);

	return out;
}

struct observation
{
	observation(const registers_t& b4, int op, int a, int b, int c, const registers_t& aft)
		: before(b4), opcode(op), a(a), b(b), c(c), after(aft) { }

	registers_t before;

	int opcode;
	int a;
	int b;
	int c;

	registers_t after;
};

struct instruction
{
	instruction(int o, int a, int b, int c) : opcode(o), a(a), b(b), c(c) { }

	int opcode;
	int a;
	int b;
	int c;
};


int main()
{
	std::vector<instruction> program;
	std::vector<observation> observations;
	{
		auto lines = util::readFileLines("input.txt");

		for(size_t i = 0; i < lines.size(); i++)
		{
			auto line = lines[i];
			if(line.find("Before") == 0)
			{
				int br0, br1, br2, br3;
				sscanf(line.c_str(), "Before: [%d, %d, %d, %d]", &br0, &br1, &br2, &br3);

				line = lines[++i];

				int op, a, b, c;
				sscanf(line.c_str(), "%d %d %d %d", &op, &a, &b, &c);

				line = lines[++i];

				int ar0, ar1, ar2, ar3;
				sscanf(line.c_str(), "After:  [%d, %d, %d, %d]", &ar0, &ar1, &ar2, &ar3);

				observations.push_back(observation({ br0, br1, br2, br3 }, op, a, b, c, { ar0, ar1, ar2, ar3 }));

				// empty line.
				line = lines[++i];
			}
			else if(line != "\n")
			{
				int op, a, b, c;
				sscanf(line.c_str(), "%d %d %d %d", &op, &a, &b, &c);

				program.push_back(instruction(op, a, b, c));
			}
		}
	}


	std::array<std::set<int>, 16> opcode_matches = { };
	std::vector<int> matches;

	for(const auto& obs : observations)
	{
		matches.push_back(0);

		for(int op = 0; op < 16; op++)
		{
			auto out = compute(obs.before, op, obs.a, obs.b, obs.c);
			if(out == obs.after)
			{
				matches.back()++;

				opcode_matches[op].insert(obs.opcode);
			}
		}
	}

	auto count = std::count_if(matches.begin(), matches.end(), [](int k) -> bool {
		return k >= 3;
	});

	tfm::printfln("part 1: %d samples match 3 or more ops", count);


	// maps from fake to real!
	std::map<int, int> real_opcode_map;

	// this one is real to fake
	std::map<int, int> fake_opcode_map;


	while(real_opcode_map.size() < 16)
	{
		for(int k = 0; k < 16; k++)
		{
			const auto& fake_matches = opcode_matches[k];

			if(fake_matches.size() == 1)
			{
				int fake = *fake_matches.begin();
				real_opcode_map[fake] = k;
				fake_opcode_map[k] = fake;

				// erase it from the rest.
				for(int m = 0; m < 16; m++)
				{
					if(m == k) continue;
					opcode_matches[m].erase(fake);
				}
			}
		}
	}


	{
		registers_t state = { 0, 0, 0, 0 };

		for(auto instr : program)
			state = compute(state, real_opcode_map[instr.opcode], instr.a, instr.b, instr.c);

		tfm::printfln("part 2: register 0 has value %d", state[0]);
	}
}









































