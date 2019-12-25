// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"
#include "assert.h"

struct State
{
	int64_t ip = 0;
	int64_t segment = 0;

	util::queue<int64_t> in;
	util::queue<int64_t> out;
	std::unordered_map<int64_t, int64_t> memory;
};

static constexpr int HALT_STOPPED = 1;
static constexpr int HALT_BLOCKED = 2;

static int run_instr(State* st)
{
	auto get = [&st](int64_t i) -> int64_t {
		return st->memory[i];
	};

	auto set_output = [&st, &get](int op, int argno, int64_t x) {

		int mode = 0;
		int64_t addr = get(st->ip + 1 + argno);

		if(argno == 0)  mode = (op % 1000)   / 100;
		if(argno == 1)  mode = (op % 10000)  / 1000;
		if(argno == 2)  mode = (op % 100000) / 10000;

		if(mode == 0)       st->memory[addr] = x;
		else if(mode == 2)  st->memory[st->segment + addr] = x;
	};

	auto get_operand = [&get, &st](int op, int argno) -> int64_t {

		int mode = 0;
		if(argno == 0)      mode = (op % 1000) / 100;
		else if(argno == 1) mode = (op % 10000) / 1000;
		else if(argno == 2) mode = (op % 100000) / 10000;
		else                abort();

		if(mode == 0)       return get(get(st->ip + 1 + argno));
		else if(mode == 1)  return get(st->ip + 1 + argno);
		else if(mode == 2)  return get(st->segment + get(st->ip + 1 + argno));
		else                abort();
	};



	int len = 0;
	int opcode = st->memory[st->ip];

	switch(opcode % 100)
	{
		case 1: {   // add
			len = 4;
			int64_t a = get_operand(opcode, 0);
			int64_t b = get_operand(opcode, 1);

			set_output(opcode, 2, a + b);
		} break;

		case 2: {   // mul
			len = 4;
			int64_t a = get_operand(opcode, 0);
			int64_t b = get_operand(opcode, 1);

			set_output(opcode, 2, a * b);
		} break;

		case 3: {   // in
			len = 2;

			if(st->in.empty())
				return HALT_BLOCKED;

			auto in = st->in.pop();
			set_output(opcode, 0, in);
		} break;

		case 4: {   // out
			len = 2;
			st->out.push(get_operand(opcode, 0));
		} break;

		case 5: {   // jmp-if-true
			len = 3;
			if(get_operand(opcode, 0) != 0)
				st->ip = get_operand(opcode, 1), len = 0;
		} break;

		case 6: {   // jmp-if-false
			len = 3;
			if(get_operand(opcode, 0) == 0)
				st->ip = get_operand(opcode, 1), len = 0;
		} break;

		case 7: {   // cmplt
			len = 4;
			int64_t a = get_operand(opcode, 0);
			int64_t b = get_operand(opcode, 1);

			set_output(opcode, 2, a < b ? 1 : 0);
		} break;

		case 8: {   // cmpeq
			len = 4;
			int64_t a = get_operand(opcode, 0);
			int64_t b = get_operand(opcode, 1);

			set_output(opcode, 2, a == b ? 1 : 0);
		} break;

		case 9: {   // set base
			len = 2;
			st->segment += get_operand(opcode, 0);
		} break;

		case 99: {
			return HALT_STOPPED;
		}
	}

	if(len > 0)
		st->ip += len;

	return 0;
}

static int run(State* st)
{
	auto x = run_instr(st);
	while(x == 0)
	{
		if(st->out.size() >= 3)
			return x;

		x = run_instr(st);
	}

	return x;
}


int main()
{
	auto ops = util::map(util::readFileLines("input.txt", ','), [](std::string_view s) -> int64_t {
		return std::stoll(std::string(s));
	});

	struct Nic
	{
		State st;
		int addr;

		bool passed = false;
		bool didPass = false;
		util::queue<std::pair<int64_t, int64_t>> packets;
	};

	struct Nat
	{
		bool first = true;
		int64_t py;

		int64_t x;
		int64_t y;
	};

	Nat nat;
	Nic nics[50];
	for(size_t n = 0; n < 50; n++)
	{
		util::foreachIdx(ops, [&nics, &n](int64_t s, size_t i) {
			nics[n].st.memory[i] = s;
		});

		nics[n].st.in.push(n);
		nics[n].addr = n;
	}

	bool didp1 = false;
	bool idles[50] = { false };

	size_t n = 0;
	while(true)
	{
		auto nic = &nics[n];
		auto r = run(&nic->st);
		if(r == HALT_BLOCKED)
		{
			// zpr::println("%d blocked (%s)", n, nic->passed);
			if(!nic->packets.empty())
			{
				auto [ x, y ] = nic->packets.pop();
				nic->st.in.push(x);
				nic->st.in.push(y);
			}
			else
			{
				if(nic->passed && !nic->didPass)
				{
					nic->st.in.push(-1);
					nic->passed = false;
					nic->didPass = true;
				}
				else if(nic->passed && nic->didPass)
				{
					idles[n] = true;
				}
				else
				{
					nic->passed = true;
				}
			}
		}
		else if(nic->st.out.size() >= 3)
		{
			auto addr = nic->st.out.pop();
			auto x = nic->st.out.pop();
			auto y = nic->st.out.pop();

			// zpr::println("%d -> %d (%d, %d)", n, addr, x, y);

			if(addr < 50)
			{
				nics[addr].packets.push({ x, y });
			}
			else if(addr == 255)
			{
				if(!didp1)
				{
					zpr::println("part 1: %d", y);
					didp1 = true;
				}

				nat.x = x;
				nat.y = y;
			}
		}

		++n %= 50;

		{
			bool allidle = true;
			for(int k = 0; k < 50; k++)
				allidle = allidle && idles[k];

			if(allidle)
			{
				nics[0].packets.push({ nat.x, nat.y });

				if(didp1 && !nat.first && nat.y == nat.py)
				{
					zpr::println("part 2: %d", nat.py);
					break;
				}

				nat.first = false;
				nat.py = nat.y;

				for(int k = 0; k < 50; k++)
				{
					idles[k] = false;
					nics[k].passed = false;
					nics[k].didPass = false;
				}
			}
		}
	}
}







