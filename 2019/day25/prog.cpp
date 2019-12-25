// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"
#include "assert.h"

#include <regex>
#include <iostream>

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

static std::vector<std::string> run(State* st, bool* finished)
{
	auto x = 0;
	while(x == 0)
		x = run_instr(st);

	if(x == HALT_STOPPED)
		*finished = true;

	std::string out;
	while(st->out.size() > 0)
		out += (char) st->out.pop();

	return util::mapFilter(util::splitString(out), [](auto sv) -> auto { return std::string(sv); },
		[](const auto& s) -> bool { return !s.empty(); });
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

	auto push_strln = [](State* st, const std::string& s) {
		for(char c : s)
			st->in.push(c);

		st->in.push('\n');
	};

	auto push_lines = [&push_strln](State* st, const std::vector<std::string>& xs, const std::string& prefix = "") {
		for(const auto& x : xs)
			push_strln(st, prefix + x);
	};





	// note: whole thing assumes no circular rooms.
	{
		std::vector<std::string> inventory;

		std::map<std::string, util::queue<std::string>> doorQueue;

		std::vector<std::string> steps;

		std::string exitDoor;   // this is the actual door to trigger the check.
		std::vector<std::string> stepsToExit;


		std::vector<std::string> currentPath;


		std::set<std::string> itemBlacklist = {
			"giant electromagnet",
			"molten lava",
			"infinite loop",
			"escape pod",
			"photons"
		};

		auto get_opp = [](const std::string& s) -> std::string {
			if(s == "north")    return "south";
			if(s == "south")    return "north";
			if(s == "east")     return "west";
			if(s == "west")     return "east";
			else                assert(false);
		};


		// { location, doors, items }
		auto parse_room = [&st, &itemBlacklist, &push_strln](const std::vector<std::string>& lines)
			-> std::tuple<std::string, std::vector<std::string>, std::vector<std::string>>
		{
			std::string location;
			std::vector<std::string> doors;
			std::vector<std::string> items;

			size_t doorsLine = 0;
			size_t itemsLine = 0;
			for(size_t i = 0; i < lines.size(); i++)
			{
				if(lines[i] == "Doors here lead:")  doorsLine = i + 1;
				if(lines[i] == "Items here:")       itemsLine = i + 1;
			}

			std::smatch sm; std::regex_search(lines[0], sm, std::regex("== ([A-Za-z0-9_ ]+) =="));
			location = sm[1];

			// if we haven't been here before:
			for(size_t i = doorsLine; i < lines.size() && lines[i].find("- ") == 0; i++)
				doors.push_back(lines[i].substr(2));

			// pickup all items:
			if(itemsLine > 0)
			{
				for(size_t i = itemsLine; i < lines.size() && lines[i].find("- ") == 0; i++)
				{
					auto item = lines[i].substr(2);
					if(itemBlacklist.find(item) == itemBlacklist.end())
						items.push_back(item);
				}
			}

			return std::tuple(location, doors, items);
		};



		// setup the initial room first.
		bool done = false;
		{
			auto lines = run(&st, &done);

			std::string loc;
			std::vector<std::string> doors;
			std::vector<std::string> items;
			std::tie(loc, doors, items) = parse_room(lines);

			doorQueue[loc].push_all(doors);
			if(!items.empty())
			{
				push_lines(&st, items, "take ");
				run(&st, &done);
			}

			currentPath.push_back(loc);
			steps.push_back("start");
		}



		while(true)
		{
			auto backtrack = [&]() {

				auto cur = currentPath.back();

				push_strln(&st, get_opp(steps.back()));
				currentPath.pop_back();
				steps.pop_back();

				run(&st, &done);
			};

			auto prev_loc = currentPath.back();

			if(doorQueue[prev_loc].empty())
			{
				// see if we're done.
				if(steps.back() == "start")
					break;

				backtrack();
				continue;
			}

			auto next = doorQueue[prev_loc].pop();
			push_strln(&st, next);

			auto lines = run(&st, &done);
			auto [ loc, doors, items ] = parse_room(lines);

			currentPath.push_back(loc);
			steps.push_back(next);

			// exclude the door that we came from
			doors = util::filter(doors, [&next, &get_opp](auto x) -> bool { return x != get_opp(next); });

			// record the path to the checkpoint.
			if(loc == "Security Checkpoint")
			{
				stepsToExit = steps;
				assert(doors.size() == 1);
				exitDoor = doors[0];

				doors.clear();
			}

			// we haven't seen this place.
			if(doorQueue.find(loc) == doorQueue.end())
			{
				// get the doors:
				doorQueue[loc].push_all(doors);

				// pickup items:
				if(!items.empty())
				{
					inventory.insert(inventory.end(), items.begin(), items.end());
					push_lines(&st, items, "take ");

					run(&st, &done);
				}
			}

			// if we reached a dead end, there'll be 0 doors since we excluded the one we came from.
			if(doors.size() == 0)
				backtrack();

			if(done)
				break;
		}

		// we should now be at the start.
		// go to the security checkpoint, and drop everything:
		push_lines(&st, stepsToExit);
		push_lines(&st, inventory, "drop ");

		run(&st, &done);

		{
			auto invCombos = util::powerset(inventory);

			for(const auto& combo : invCombos)
			{
				push_lines(&st, combo, "take ");
				push_strln(&st, exitDoor);
				push_lines(&st, combo, "drop ");

				auto lines = run(&st, &done);

				for(const auto& l : lines)
				{
					if(l.find("get in by typing") != std::string::npos)
					{
						std::smatch sm; std::regex_search(l, sm, std::regex("get in by typing (\\d+) on the keypad"));
						auto password = sm[1].str();

						zpr::println("part 1: %s", password);
						zpr::println("part 2: -");
						exit(0);
					}
				}
			}
		}
	}
}




