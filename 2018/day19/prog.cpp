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

#include "aoc.h"
#include "tinyformat.h"


using registers_t = std::array<int, 6>;



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
		auto lines = util::readFileLines("input.txt");
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

		for(int ip = 0;;)
		{
			if(ip >= program.size())
				break;

			auto instr = program[ip];
			state[ipreg] = ip;

			state = compute(state, instr.opcode, instr.a, instr.b, instr.c);

			ip = state[ipreg];

			// tfm::printfln("state: ip(%02d), [ %d, %d, %d, %d, %d, %d ]", ip, state[0], state[1], state[2], state[3], state[4], state[5]);

			ip++;
		}

		tfm::printfln("part 1: register 0 has value %d", state[0]);
	}


	{
		registers_t state = { 1, 0, 0, 0, 0, 0 };

		for(int ip = 0, clk = 0;; clk++)
		{
			if(ip >= program.size())
				break;

			// if we've reached IP = 1 then we're done with the setup.
			if(ip == 2)
				break;

			{
				auto instr = program[ip];
				state[ipreg] = ip;

				state = compute(state, instr.opcode, instr.a, instr.b, instr.c);

				ip = state[ipreg];
				ip++;
			}
		}

		int sum = 0;
		int target = state[3];

		for(int i = 1; i <= target; i++)
		{
			if(target % i == 0)
				sum += i;
		}

		tfm::printfln("part 2: sum of factors of %d = %d", target, sum);
	}
}


/*
	A B C D E IP

	# ip 5
	0   addi 5 16 5         # IP = IP + 16          # GOTO 17

	# after the bottom bit, we get:
	# D = 10551373, E = 10550400
	# i think the initial value of E is irrelevant.

	1   seti 1 8 2          # C = 1                 # C = 1
	2   seti 1 1 1          # B = 1                 # B = 1
	3   mulr 2 1 4          # E = B * C             # E = B * C
	4   eqrr 4 3 4          # E = (D == E)          # E = (D == E)
	5   addr 4 5 5          # IP = E + IP           # IF(D == E) GOTO 7 ELSE GOTO 6
	6   addi 5 1 5          # IP = IP + 1           # GOTO 8
	7   addr 2 0 0          # A = A + C             # A += C
	8   addi 1 1 1          # B = B + 1             # B += 1
	9   gtrr 1 3 4          # E = (B > D)           # E = (B > D)
	10  addr 5 4 5          # IP = IP + E           # IF(B > D) GOTO 12 ELSE GOTO 11
	11  seti 2 8 5          # IP = C                # GOTO 3
	12  addi 2 1 2          # C = C + 1             # C += 1
	13  gtrr 2 3 4          # E = (C > D)           # E = (C > D)
	14  addr 4 5 5          # IP = IP + E           # IF(C > D) GOTO 16 ELSE GOTO 15
	15  seti 1 7 5          # IP = 1                # GOTO 2
	16  mulr 5 5 5          # IP = IP * IP          # EXIT


	C = 1
	B = 1
	while(true) # probably
	{
		E = B * C
		if(D == E)
		{
			# the output increases by C. what is C?
			# no idea.
			A += C
		}

		B += 1
		# what is this?
		if(B > D)
		{
			C += 1

			# or, if C == D we break.
			# if not we continue...?

			if(C > D)
			{
				break
			}
			else
			{
				# reset B...
				B = 1
				continue
			}
		}
		else
		{
			continue
		}
	}


	# probably this:
	for B in 1 .. 10551373
	{
		for C in 1 .. 10551373
		{
			if B * C == 10551373
				A += C
		}
	}

	# fuck lah probably factorisation again
	# sum of factors



	# i think all this junk below just sets up the starting values
	17  addi 3 2 3          # D = D + 2             #
	18  mulr 3 3 3          # D = D * D             #
	19  mulr 5 3 3          # D = IP * D            #
	20  muli 3 11 3         # D = D * 11            #
	21  addi 4 6 4          # E = E + 6             #
	22  mulr 4 5 4          # E = E * IP            #
	23  addi 4 5 4          # E = E + 5             #
	24  addr 3 4 3          # D = D + E             #
	25  addr 5 0 5          # IP = IP + A           #
	26  seti 0 0 5          # IP = 0                #
	27  setr 5 3 4          # E = IP                #
	28  mulr 4 5 4          # E = E * 5             #
	29  addr 5 4 4          # E = IP + 4            #
	30  mulr 5 4 4          # E = IP * 4            #
	31  muli 4 14 4         # E = E * 14            #
	32  mulr 4 5 4          # E = E * IP            #
	33  addr 3 4 3          # D = D + E             #
	34  seti 0 3 0          # A = 0                 #
	35  seti 0 0 5          # IP = 0                # GOTO 1

*/





































