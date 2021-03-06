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

static inline int64_t get(State* st, int64_t i)
{
	return st->memory[i];
}

static inline void set_output(State* st, int op, int argno, int64_t x)
{
	int mode = 0;
	int64_t addr = get(st, st->ip + 1 + argno);

	if(argno == 0)  mode = (op % 1000)   / 100;
	if(argno == 1)  mode = (op % 10000)  / 1000;
	if(argno == 2)  mode = (op % 100000) / 10000;

	if(mode == 0)       st->memory[addr] = x;
	else if(mode == 2)  st->memory[st->segment + addr] = x;
}

static inline int64_t get_operand(State* st, int op, int argno)
{
	int mode = 0;
	if(argno == 0)      mode = (op % 1000) / 100;
	else if(argno == 1) mode = (op % 10000) / 1000;
	else if(argno == 2) mode = (op % 100000) / 10000;
	else                abort();

	if(mode == 0)       return get(st, get(st, st->ip + 1 + argno));
	else if(mode == 1)  return get(st, st->ip + 1 + argno);
	else if(mode == 2)  return get(st, st->segment + get(st, st->ip + 1 + argno));
	else                abort();
}

static int run_instr(State* st)
{
	int len = 0;
	int opcode = st->memory[st->ip];

	switch(opcode % 100)
	{
		case 1: {   // add
			len = 4;
			int64_t a = get_operand(st, opcode, 0);
			int64_t b = get_operand(st, opcode, 1);

			set_output(st, opcode, 2, a + b);
		} break;

		case 2: {   // mul
			len = 4;
			int64_t a = get_operand(st, opcode, 0);
			int64_t b = get_operand(st, opcode, 1);

			set_output(st, opcode, 2, a * b);
		} break;

		case 3: {   // in
			len = 2;

			if(st->in.empty())
				return HALT_BLOCKED;

			auto in = st->in.pop();
			set_output(st, opcode, 0, in);
		} break;

		case 4: {   // out
			len = 2;
			st->out.push(get_operand(st, opcode, 0));
		} break;

		case 5: {   // jmp-if-true
			len = 3;
			if(get_operand(st, opcode, 0) != 0)
				st->ip = get_operand(st, opcode, 1), len = 0;
		} break;

		case 6: {   // jmp-if-false
			len = 3;
			if(get_operand(st, opcode, 0) == 0)
				st->ip = get_operand(st, opcode, 1), len = 0;
		} break;

		case 7: {   // cmplt
			len = 4;
			int64_t a = get_operand(st, opcode, 0);
			int64_t b = get_operand(st, opcode, 1);

			set_output(st, opcode, 2, a < b ? 1 : 0);
		} break;

		case 8: {   // cmpeq
			len = 4;
			int64_t a = get_operand(st, opcode, 0);
			int64_t b = get_operand(st, opcode, 1);

			set_output(st, opcode, 2, a == b ? 1 : 0);
		} break;

		case 9: {   // set base
			len = 2;
			st->segment += get_operand(st, opcode, 0);
		} break;

		case 99: {
			return HALT_STOPPED;
		}
	}

	if(len > 0)
		st->ip += len;

	return 0;
}

static int run(State st, int x, int y)
{
	st.in.push(x);
	st.in.push(y);

	auto r = run_instr(&st);
	while(r != HALT_STOPPED)
		r = run_instr(&st);

	return st.out.pop();
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


	{
		int sum = 0;

		std::map<v2, int> map;
		for(int i = 0; i < 50; i++)
		{
			for(int j = 0; j < 50; j++)
			{
				auto q = run(st, i, j);
				map[v2(i, j)] = q;

				sum += q;
			}
		}

		zpr::println("part 1: %d", sum);
	}


	{
		constexpr int width = 100;

		auto check = [&st](v2 v) -> bool {
			return run(st, v.x, v.y);
		};

		auto check_box = [&st, &check](v2 v) -> bool {
			return check(v) && check(v2(v.x, v.y - width + 1))
				&& check(v2(v.x + width - 1, v.y))
				&& check(v2(v.x + width - 1, v.y - width + 1));
		};


		v2 lower_left = v2(0, width - 1);
		while(true)
		{
			while(!check(lower_left))
				lower_left.x++;

			if(check_box(lower_left))
				break;

			else
				lower_left.y++;
		}

		auto pos = lower_left - v2(0, width - 1);
		zpr::println("part 2: %d", pos.x * 10000 + pos.y);
	}
}





