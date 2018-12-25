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
#include <thread>
#include <string>
#include <vector>
#include <fstream>
#include <functional>
#include <unordered_set>

#include "utils.h"
#include "tinyformat.h"


using registers_t = std::array<uint64_t, 6>;



registers_t compute(const registers_t& before, int real_opcode, uint64_t a, uint64_t b, uint64_t c)
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

struct instruction
{
	instruction(int o, uint64_t a, uint64_t b, uint64_t c) : opcode(o), a(a), b(b), c(c) { }

	int opcode;
	uint64_t a;
	uint64_t b;
	uint64_t c;
};

int ipreg = 0;

int main()
{
	std::map<std::string, int> opcodes;
	opcodes["addr"] = 0;
	opcodes["addi"] = 1;
	opcodes["mulr"] = 2;
	opcodes["muli"] = 3;
	opcodes["banr"] = 4;
	opcodes["bani"] = 5;
	opcodes["borr"] = 6;
	opcodes["bori"] = 7;
	opcodes["setr"] = 8;
	opcodes["seti"] = 9;
	opcodes["gtir"] = 10;
	opcodes["gtri"] = 11;
	opcodes["gtrr"] = 12;
	opcodes["eqir"] = 13;
	opcodes["eqri"] = 14;
	opcodes["eqrr"] = 15;

	std::vector<instruction> program;
	{
		auto lines = util::readFileLines("input2.txt");
		for(size_t i = 0; i < lines.size(); i++)
		{
			auto line = lines[i];
			if(line[0] == '#')
			{
				sscanf(line.c_str(), "#ip %d", &ipreg);
			}
			else if(line != "\n")
			{
				int a, b, c;
				char _op[5];
				sscanf(line.c_str(), "%s %d %d %d", _op, &a, &b, &c);

				auto op = std::string(_op);
				program.push_back(instruction(opcodes[op], a, b, c));
			}
		}
	}

	{
		registers_t state = { 0, 0, 0, 0, 0, 0 };

		int count = 0;
		int prev_reg = 0;
		std::set<int> seen;

		for(int ip = 0;;)
		{
			if(ip >= program.size())
				break;

			auto instr = program[ip];
			state[ipreg] = ip;

			if(ip == 28)
			{
				int reg = state[instr.a];
				if(seen.empty())
				{
					tfm::printfln("part 1: r0 = %d, halts after %d instructions", reg, count + 2);
					seen.insert(reg);
				}
				else
				{
					if(seen.find(reg) == seen.end())
					{
						seen.insert(reg);
						prev_reg = reg;
					}
					else
					{
						tfm::printfln("part 2: r0 = %d", prev_reg);
						break;
					}
				}
			}

			state = compute(state, instr.opcode, instr.a, instr.b, instr.c);

			ip = state[ipreg];

			ip++;
			count++;
		}
	}
}
















