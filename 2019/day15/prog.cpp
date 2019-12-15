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

static int run(State* st, int input)
{
	st->in.push(input);

	while(st->out.empty())
		run_instr(st);

	return st->out.pop();
}



constexpr int STATUS_WALL = 0;
constexpr int STATUS_OK   = 1;
constexpr int STATUS_GOAL = 2;
constexpr int STATUS_OXY  = 3;

static v2 goal_pos;

static void explore(State* st, v2 pos, std::map<v2, int>& map, int indt = 0)
{
	if(map[pos] == STATUS_GOAL)
		goal_pos = pos;

	// north=1 south=2 west=3 east=4
	{
		// north:
		v2 new_pos = pos + v2(0, -1);
		if(map.find(new_pos) == map.end())
		{
			int status = run(st, 1);

			map[new_pos] = status;
			if(status == STATUS_OK || status == STATUS_GOAL)
			{
				explore(st, new_pos, map, indt + 1);

				// backtrack south.
				run(st, 2);
			}
		}
	}

	{
		// south:
		v2 new_pos = pos + v2(0, 1);
		if(map.find(new_pos) == map.end())
		{
			int status = run(st, 2);

			map[new_pos] = status;
			if(status == STATUS_OK || status == STATUS_GOAL)
			{
				explore(st, new_pos, map, indt + 1);

				// backtrack north.
				run(st, 1);
			}
		}
	}

	{
		// west:
		v2 new_pos = pos + v2(-1, 0);
		if(map.find(new_pos) == map.end())
		{
			int status = run(st, 3);

			map[new_pos] = status;
			if(status == STATUS_OK || status == STATUS_GOAL)
			{
				explore(st, new_pos, map, indt + 1);

				// backtrack east.
				run(st, 4);
			}
		}
	}

	{
		// east:
		v2 new_pos = pos + v2(1, 0);
		if(map.find(new_pos) == map.end())
		{
			int status = run(st, 4);

			map[new_pos] = status;
			if(status == STATUS_OK || status == STATUS_GOAL)
			{
				explore(st, new_pos, map, indt + 1);

				// backtrack east.
				run(st, 3);
			}
		}
	}
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


	// explore the entire map first?
	std::map<v2, int> map;
	map[v2(0, 0)] = 1;

	explore(&st, v2(0, 0), map);


	auto get_adj = [&map](const v2& pos) -> std::vector<v2> {
		std::vector<v2> adj;
		if(map[pos + v2(0,  1)] != STATUS_WALL) adj.push_back(pos + v2(0, 1));
		if(map[pos + v2(0, -1)] != STATUS_WALL) adj.push_back(pos + v2(0, -1));
		if(map[pos + v2(1,  0)] != STATUS_WALL) adj.push_back(pos + v2(1, 0));
		if(map[pos + v2(-1, 0)] != STATUS_WALL) adj.push_back(pos + v2(-1, 0));

		return adj;
	};



	{
		auto path = util::astarSearch(v2(0, 0), goal_pos, get_adj, [](const v2& a, const v2& b) -> int {
			return util::manhattan_dist(a, b);
		});

		zpr::println("part 1: %d", path.size() - 1);
	}


	{
		std::vector<v2> queue;
		queue.push_back(goal_pos);

		int timer = -1;

		while(!queue.empty())
		{
			timer++;

			auto q = queue;
			queue.clear();

			for(auto x : q)
			{
				map[x] = STATUS_OXY;
				auto xs = util::filter(get_adj(x), [&map](const v2& x) -> bool {
					return map[x] != STATUS_OXY;
				});

				queue.insert(queue.end(), xs.begin(), xs.end());
			}
		}

		zpr::println("part 2: %d", timer);
	}
}













