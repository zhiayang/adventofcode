// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"
#include "assert.h"

struct State
{
	int ip = 0;

	util::queue<int> in;
	util::queue<int> out;
	std::map<int, int> memory;
};

static constexpr int HALT_STOPPED = 1;
static constexpr int HALT_BLOCKED = 2;

static int run_instr(State* st)
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
			// 10000s digit
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

			if(st->in.empty())
				return HALT_BLOCKED;

			auto in = st->in.pop();
			set(get(st->ip + 1), in);
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

		case 99: {
			return HALT_STOPPED;
		}
	}

	if(len > 0)
		st->ip += len;

	return 0;
}

static int run_p1(State st, int input, int phase)
{
	st.ip = 0;
	st.in.push(phase);
	st.in.push(input);

	while(st.memory[st.ip] != 99)
		run_instr(&st);

	return st.out.front();
}

static int run_p2(State* st)
{
	int x = 0;
	while((x = run_instr(st)) == 0)
		;

	return x;
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

	struct Amp
	{
		Amp() { }
		Amp(int a, int b, int c, int d, int e) : a(a), b(b), c(c), d(d), e(e) { }
		int a = 0;
		int b = 0;
		int c = 0;
		int d = 0;
		int e = 0;
	};


	{
		auto settings = util::map(util::permutations(std::vector<int>{ 0, 1, 2, 3, 4 }), [](const std::vector<int>& sets) -> Amp {
			return Amp(sets[0], sets[1], sets[2], sets[3], sets[4]);
		});

		int max_out = 0;
		for(const auto& set : settings)
		{
			int a1 = run_p1(st, 0, set.a);
			int a2 = run_p1(st, a1, set.b);
			int a3 = run_p1(st, a2, set.c);
			int a4 = run_p1(st, a3, set.d);
			int a5 = run_p1(st, a4, set.e);

			max_out = std::max(max_out, a5);
		}

		zpr::println("part 1: %d", max_out);
	}


	{
		auto settings = util::map(util::permutations(std::vector<int>{ 5, 6, 7, 8, 9 }), [](const std::vector<int>& sets) -> Amp {
			return Amp(sets[0], sets[1], sets[2], sets[3], sets[4]);
		});

		int max_out = 0;
		for(const auto& set : settings)
		{
			auto amp1 = st; amp1.in.push(set.a);
			auto amp2 = st; amp2.in.push(set.b);
			auto amp3 = st; amp3.in.push(set.c);
			auto amp4 = st; amp4.in.push(set.d);
			auto amp5 = st; amp5.in.push(set.e);

			util::queue<State*> amp_queue = { &amp1, &amp2, &amp3, &amp4, &amp5 };

			int output = 0;
			while(!amp_queue.empty())
			{
				auto amp = amp_queue.pop();

				amp->in.push(output);
				int x = run_p2(amp);

				if(x == HALT_BLOCKED)
					amp_queue.push(amp);

				output = amp->out.pop();
			}

			max_out = std::max(output, max_out);
		}

		zpr::println("part 2: %d", max_out);
	}
}










