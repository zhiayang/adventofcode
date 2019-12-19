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

static std::map<v2, int> run(State* st)
{
	auto x = run_instr(st);
	while(x != HALT_STOPPED)
		x = run_instr(st);

	std::map<v2, int> ret;
	v2 pos;
	while(!st->out.empty())
	{
		if(st->out.front() < 127)
		{
			auto c = st->out.pop();
			if(c == 10)
			{
				pos.x = 0;
				pos.y++;
			}
			else
			{
				ret[pos] = c;
				pos += v2(1, 0);
			}
		}
		else
		{
			break;
		}
	}

	return ret;
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
		auto copy = st;

		auto map = run(&copy);
		int sum = 0;

		// get intersections.
		for(const auto p : map)
		{
			auto pos = p.first;
			if(map[pos] == '#' && map[pos + v2(1, 0)] == '#' && map[pos + v2(-1, 0)] == '#'
				&& map[pos + v2(0, 1)] == '#' && map[pos + v2(0, -1)] == '#')
			{
				sum += pos.x * pos.y;
			}
		}

		zpr::println("part 1: %d", sum);
	}


	{
		// this is dumb.
		std::string main = "A,B,B,C,C,A,A,B,B,C\n";
		std::string A = "L,12,R,4,R,4\n";
		std::string B = "R,12,R,4,L,12\n";
		std::string C = "R,12,R,4,L,6,L,8,L,8\n";

		st.memory[0] = 2;
		for(char c : main)
			st.in.push(c);

		for(char c : A)
			st.in.push(c);

		for(char c : B)
			st.in.push(c);

		for(char c : C)
			st.in.push(c);

		st.in.push('n');
		st.in.push('\n');

		auto map = run(&st);

		zpr::println("part 2: %d", st.out.pop());
	}
}





