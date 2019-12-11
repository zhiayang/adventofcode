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
	std::map<int64_t, int64_t> memory;
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

static std::pair<int, int> run(State* st, int input)
{
	st->in.push(input);

	auto x = 0;
	while(x == 0)
		x = run_instr(st);

	if(x == HALT_STOPPED)
		return { -1, -1 };

	else
	{
		int a = st->out.pop();
		int b = st->out.pop();
		return std::make_pair(a, b);
	}
}

static std::map<v2, int> run_robot(State st, int start, v2* tlCorner = 0, v2* brCorner = 0)
{
	auto position = v2(0, 0);

	// (1, 1) is right-down.
	v2 dir = v2(0, -1);

	std::map<v2, int> paints;
	paints[position] = start;

	while(true)
	{
		// some nonsense to prevent increasing the size, because we might not actually paint the last square.
		auto [ colour, turn ] = run(&st, paints.find(position) == paints.end() ? 0 : paints[position]);
		if(colour == -1)
			break;

		paints[position] = colour;
		if(turn == 0)
		{
			// left
			if(dir == v2(0, -1))        dir = v2(-1, 0);
			else if(dir == v2(-1, 0))   dir = v2(0, +1);
			else if(dir == v2(0, +1))   dir = v2(+1, 0);
			else if(dir == v2(+1, 0))   dir = v2(0, -1);
		}
		else if(turn == 1)
		{
			// right
			if(dir == v2(0, -1))        dir = v2(+1, 0);
			else if(dir == v2(-1, 0))   dir = v2(0, -1);
			else if(dir == v2(0, +1))   dir = v2(-1, 0);
			else if(dir == v2(+1, 0))   dir = v2(0, +1);
		}

		if(tlCorner && brCorner)
		{
			tlCorner->x = std::min(position.x, tlCorner->x);
			tlCorner->y = std::min(position.y, tlCorner->y);

			brCorner->x = std::max(position.x, brCorner->x);
			brCorner->y = std::max(position.y, brCorner->y);
		}

		position += dir;
	}

	return paints;
}


int main()
{
	auto ops = util::map(util::readFileLines("input.txt", ','), [](std::string_view s) -> int64_t {
		return std::stoll(std::string(s));
	});

	State st;
	util::foreachIdx(ops, [&st](int64_t s, size_t i) {
		st.memory[i] = s;
	});


	zpr::println("part 1: %d", run_robot(st, 0).size());

	{
		v2 tlCorner;
		v2 brCorner;

		auto paints = run_robot(st, 1, &tlCorner, &brCorner);

		zpr::println("part 2:");
		for(int y = tlCorner.y; y <= brCorner.y; y++)
		{
			for(int x = tlCorner.x; x <= brCorner.x; x++)
			{
				auto pos = v2(x, y);
				zpr::print("%c", paints[pos] == 1 ? '#' : ' ');
			}
			zpr::print("\n");
		}
	}
}










