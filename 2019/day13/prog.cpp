// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"
#include "assert.h"

#include <thread>

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

static int run(State* st)
{
	auto x = 0;
	while(x == 0)
		x = run_instr(st);

	return x;
}

struct Game
{
	int score = 0;
	int blocks = 0;

	v2 paddle_pos;

	v2 ball_pos;
	v2 prev_ball_pos;
};

static int run_game(State* st, Game* game, std::map<v2, int>& map, v2* tlCorner, v2* brCorner)
{
	int ret = run(st);

	while(st->out.size() > 0)
	{
		int a = st->out.pop();
		int b = st->out.pop();
		int c = st->out.pop();

		auto pos = v2(a, b);

		if(a == -1 && b == 0)
		{
			game->score = c;
		}
		else
		{
			if(c == 0)
			{
				if(map[pos] == 2)
					game->blocks--;
			}
			else if(c == 2)
			{
				if(map[pos] != c)
					game->blocks++;
			}
			else if(c == 3)
			{
				game->paddle_pos = pos;
			}
			else if(c == 4)
			{
				game->prev_ball_pos = game->ball_pos;
				game->ball_pos = pos;
			}

			map[pos] = c;
			tlCorner->x = std::min(pos.x, tlCorner->x);
			tlCorner->y = std::min(pos.y, tlCorner->y);

			brCorner->x = std::max(pos.x, brCorner->x);
			brCorner->y = std::max(pos.y, brCorner->y);
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

	st.memory[0] = 2;

	v2 tlCorner;
	v2 brCorner;

	Game game;
	std::map<v2, int> map;
	int state = run_game(&st, &game, map, &tlCorner, &brCorner);

	int part1 = game.blocks;

	while(state != 0 && game.blocks > 0)
	{
		int input = 0;
		if(game.ball_pos.x < game.paddle_pos.x)
			input = -1;

		if(game.ball_pos.x > game.paddle_pos.x)
			input = 1;

		st.in.push(input);
		state = run_game(&st, &game, map, &tlCorner, &brCorner);


		#if LIVE_UPDATE
		{
			for(int y = tlCorner.y; y <= brCorner.y; y++)
			{
				for(int x = tlCorner.x; x <= brCorner.x; x++)
				{
					auto pos = v2(x, y);
					zpr::print("%c", map[pos] == 0
										? ' '
									: map[pos] == 1
										? '@'
									: map[pos] == 2
										? '#'
									: map[pos] == 3
										? '='
									: map[pos] == 4
										? '*'
										: '?');
				}
				zpr::print("\n");
			}

			zpr::println("score: %d   |   %d blocks left     ", game.score, game.blocks);

			std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(10));
			zpr::print("\x1b[3J\x1b[1;1H");
		}
		#endif
	}

	zpr::print("\x1b[3J\x1b[1;1H\n\n");
	zpr::println("part 1: %d", part1);
	zpr::println("part 2: %d", game.score);
}












