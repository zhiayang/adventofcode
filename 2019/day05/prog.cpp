// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"
#include "assert.h"

struct State
{
	int ip = 0;
	int io = 0;

	std::map<int, int> memory;
};

static void run_instr(State* st)
{
	auto get = [&st](int i) -> int {
		return st->memory[i];
	};

	auto set = [&st](int i, int x) {
		st->memory[i] = x;
	};

	auto get_operand = [&get, &st](int op, int i) -> int {

		int mode = 0;
		if(i == 0)
		{
			// 100s digit.
			mode = (op % 1000) / 100;
		}
		else if(i == 1)
		{
			// 1000s digit
			mode = (op % 10000) / 1000;
		}
		else if(i == 2)
		{
			// 1000s digit
			mode = (op % 100000) / 10000;
		}
		else
		{
			assert("unsupported!");
		}

		if(mode == 0)
		{
			// position mode (memory operand)
			return get(get(st->ip + 1 + i));
		}
		else
		{
			// immediate operand
			return get(st->ip + 1 + i);
		}
	};

	int len = 0;
	int opcode = st->memory[st->ip];

	switch(opcode % 100)
	{
		case 1: {   // add
			len = 4;
			int a = get_operand(opcode, 0);
			int b = get_operand(opcode, 1);

			set(get(st->ip + 3), a + b);
		} break;

		case 2: {   // mul
			len = 4;
			int a = get_operand(opcode, 0);
			int b = get_operand(opcode, 1);

			set(get(st->ip + 3), a * b);
		} break;

		case 3: {   // in
			len = 2;
			set(get(st->ip + 1), st->io);
		} break;

		case 4: {   // out
			len = 2;
			st->io = get_operand(opcode, 0);
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
			int a = get_operand(opcode, 0);
			int b = get_operand(opcode, 1);

			set(get(st->ip + 3), a < b ? 1 : 0);
		} break;

		case 8: {   // cmpeq
			len = 4;
			int a = get_operand(opcode, 0);
			int b = get_operand(opcode, 1);

			set(get(st->ip + 3), a == b ? 1 : 0);
		} break;
	}

	if(len > 0)
		st->ip += len;
}

static State run(State st, int input)
{
	st.io = input;
	while(st.memory[st.ip] != 99)
		run_instr(&st);

	return st;
}

int main()
{
	auto ops = util::map(util::readFileLines("input.txt", ','), [](std::string_view s) -> int {
		return std::stoi(std::string(s));
	});

	State st;
	util::foreachIdx(ops, [&st](int s, size_t i) {
		st.memory[i] = s;
	});

	{
		auto res = run(st, 1);
		zpr::println("part 1: %d", res.io);
	}

	{
		auto res = run(st, 5);
		zpr::println("part 2: %d", res.io);
	}
}








