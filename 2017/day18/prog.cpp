// prog.cpp
// Copyright (c) 2014 - 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <set>
#include <map>
#include <deque>
#include <array>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include <algorithm>
#include <string_view>

int main()
{
	std::vector<std::string_view> input = {
		/* 0 */  "set i 31",
		/* 1 */  "set a 1",
		/* 2 */  "mul p 17",
		/* 3 */  "jgz p p",
		/* 4 */  "mul a 2",
		/* 5 */  "add i -1",
		/* 6 */  "jgz i -2",
		/* 7 */  "add a -1",
		/* 8 */  "set i 127",
		/* 9 */  "set p 826",
		/* 10 */ "mul p 8505",
		/* 11 */ "mod p a",
		/* 12 */ "mul p 129749",
		/* 13 */ "add p 12345",
		/* 14 */ "mod p a",
		/* 15 */ "set b p",
		/* 16 */ "mod b 10000",
		/* 17 */ "snd b",
		/* 18 */ "add i -1",
		/* 19 */ "jgz i -9",
		/* 20 */ "jgz a 3",
		/* 21 */ "rcv b",
		/* 22 */ "jgz b -1",
		/* 23 */ "set f 0",
		/* 24 */ "set i 126",
		/* 25 */ "rcv a",
		/* 26 */ "rcv b",
		/* 27 */ "set p a",
		/* 28 */ "mul p -1",
		/* 29 */ "add p b",
		/* 30 */ "jgz p 4",
		/* 31 */ "snd a",
		/* 32 */ "set a b",
		/* 33 */ "jgz 1 3",
		/* 34 */ "snd b",
		/* 35 */ "set f 1",
		/* 36 */ "add i -1",
		/* 37 */ "jgz i -11",
		/* 38 */ "snd a",
		/* 39 */ "jgz f -16",
		/* 40 */ "jgz a -19"
	};

	#if 0

	std::array<int64_t, 26> regs;
	std::vector<int64_t> sounds;

	for(int i = 0; i < input.size(); )
	{
		auto instr = input[i];

		// printf("%d: ", i);

		if(instr.find("add") == 0)
		{
			instr.remove_prefix(4);
			char reg = instr[0];

			instr.remove_prefix(2);
			int val = 0;
			if(isalpha(instr[0]))
				val = regs[instr[0] - 'a'];

			else
				val = std::stoi(std::string(instr.data(), instr.length()));

			regs[reg - 'a'] += val;
			// printf("%c += %d == %d\n", reg, val, regs[reg - 'a']);
		}
		else if(instr.find("set") == 0)
		{
			instr.remove_prefix(4);
			char reg = instr[0];

			instr.remove_prefix(2);
			int val = 0;
			if(isalpha(instr[0]))
				val = regs[instr[0] - 'a'];

			else
				val = std::stoi(std::string(instr.data(), instr.length()));

			regs[reg - 'a'] = val;
			// printf("%c = %d\n", reg, val);
		}
		else if(instr.find("mul") == 0)
		{
			instr.remove_prefix(4);
			char reg = instr[0];

			instr.remove_prefix(2);
			int val = 0;
			if(isalpha(instr[0]))
				val = regs[instr[0] - 'a'];

			else
				val = std::stoi(std::string(instr.data(), instr.length()));

			regs[reg - 'a'] *= val;
			// printf("%c *= %d == %d\n", reg, val, regs[reg - 'a']);
		}
		else if(instr.find("mod") == 0)
		{
			instr.remove_prefix(4);
			char reg = instr[0];

			instr.remove_prefix(2);
			int val = 0;
			if(isalpha(instr[0]))
				val = regs[instr[0] - 'a'];

			else
				val = std::stoi(std::string(instr.data(), instr.length()));

			regs[reg - 'a'] %= val;
			// printf("%c %%= %d == %d\n", reg, val, regs[reg - 'a']);
		}
		else if(instr.find("jgz") == 0)
		{
			instr.remove_prefix(4);

			int cond = 0;
			if(isalpha(instr[0]))
			{
				cond = regs[instr[0] - 'a'], instr.remove_prefix(2);
			}
			else
			{
				size_t p = 0;
				cond = std::stoi(std::string(instr.data(), instr.length()), &p);
				instr.remove_prefix(p + 1);
			}

			int jmp = 0;
			if(isalpha(instr[0]))
			{
				jmp = regs[instr[0] - 'a'], instr.remove_prefix(2);
			}
			else
			{
				size_t p = 0;
				jmp = std::stoi(std::string(instr.data(), instr.length()), &p);
				instr.remove_prefix(p + 1);
			}

			// printf("jgz (%d) %d\n", cond, jmp);
			if(cond > 0)
			{
				i += jmp;
				continue;
			}
		}
		else if(instr.find("snd") == 0)
		{
			instr.remove_prefix(4);

			// ok...
			int sound = 0;

			if(isalpha(instr[0]))
				sound = regs[instr[0] - 'a'];

			else
				sound = std::stoi(std::string(instr.data(), instr.length()));

			// printf("play sound %d\n", sound);
			sounds.push_back(sound);
		}
		else if(instr.find("rcv") == 0)
		{
			instr.remove_prefix(4);

			int val = 0;

			// ok...
			if(isalpha(instr[0]))
				val = regs[instr[0] - 'a'];

			else
				val = std::stoi(std::string(instr.data(), instr.length()));

			if(val != 0)
			{
				printf("recovered %lld\n", sounds.back());
				break;
			}
		}

		i++;
	}

	#else

	std::array<std::array<int64_t, 26>, 2> regs;
	std::array<std::deque<int64_t>, 2> msgQueue;

	regs[0]['p'] = 0;
	regs[1]['p'] = 1;

	int onesent = 0;

	auto process = [&regs, &msgQueue, &input, &onesent](int p, int ip) -> std::pair<int64_t, bool> {

		auto instr = input[ip];

		// printf("%d: ", i);

		if(instr.find("add") == 0)
		{
			instr.remove_prefix(4);
			char reg = instr[0];

			instr.remove_prefix(2);
			int val = 0;
			if(isalpha(instr[0]))
				val = regs[p][instr[0] - 'a'];

			else
				val = std::stoi(std::string(instr.data(), instr.length()));

			regs[p][reg - 'a'] += val;
			// printf("%c += %d == %d\n", reg, val, regs[reg - 'a']);
		}
		else if(instr.find("set") == 0)
		{
			instr.remove_prefix(4);
			char reg = instr[0];

			instr.remove_prefix(2);
			int val = 0;
			if(isalpha(instr[0]))
				val = regs[p][instr[0] - 'a'];

			else
				val = std::stoi(std::string(instr.data(), instr.length()));

			regs[p][reg - 'a'] = val;
			// printf("%c = %d\n", reg, val);
		}
		else if(instr.find("mul") == 0)
		{
			instr.remove_prefix(4);
			char reg = instr[0];

			instr.remove_prefix(2);
			int val = 0;
			if(isalpha(instr[0]))
				val = regs[p][instr[0] - 'a'];

			else
				val = std::stoi(std::string(instr.data(), instr.length()));

			regs[p][reg - 'a'] *= val;
			// printf("%c *= %d == %d\n", reg, val, regs[p][reg - 'a']);
		}
		else if(instr.find("mod") == 0)
		{
			instr.remove_prefix(4);
			char reg = instr[0];

			instr.remove_prefix(2);
			int val = 0;
			if(isalpha(instr[0]))
				val = regs[p][instr[0] - 'a'];

			else
				val = std::stoi(std::string(instr.data(), instr.length()));

			regs[p][reg - 'a'] %= val;
			// printf("%c %%= %d == %d\n", reg, val, regs[p][reg - 'a']);
		}
		else if(instr.find("jgz") == 0)
		{
			instr.remove_prefix(4);

			int cond = 0;
			if(isalpha(instr[0]))
			{
				cond = regs[p][instr[0] - 'a'], instr.remove_prefix(2);
			}
			else
			{
				size_t p = 0;
				cond = std::stoi(std::string(instr.data(), instr.length()), &p);
				instr.remove_prefix(p + 1);
			}

			int jmp = 0;
			if(isalpha(instr[0]))
			{
				jmp = regs[p][instr[0] - 'a'], instr.remove_prefix(2);
			}
			else
			{
				size_t p = 0;
				jmp = std::stoi(std::string(instr.data(), instr.length()), &p);
				instr.remove_prefix(p + 1);
			}

			// printf("jgz (%d) %d\n", cond, jmp);
			if(cond > 0)
				return { jmp, false };
		}
		else if(instr.find("snd") == 0)
		{
			instr.remove_prefix(4);

			// ok...
			int sound = 0;

			if(isalpha(instr[0]))
				sound = regs[p][instr[0] - 'a'];

			else
				sound = std::stoi(std::string(instr.data(), instr.length()));

			msgQueue[p ^ 1].push_back(sound);

			if(p == 1) onesent++;
			return { 1, true };
		}
		else if(instr.find("rcv") == 0)
		{
			instr.remove_prefix(4);

			// ok...
			assert(isalpha(instr[0]));
			char reg = instr[0];

			if(msgQueue[p].size() > 0)
			{
				regs[p][reg - 'a'] = msgQueue[p].front();
				msgQueue[p].pop_front();
			}
			else
			{
				return { 0, true };
			}
		}

		return { 1, false };
	};


	int prog = 0;
	std::array<int, 2> ips;
	std::array<int, 2> died;
	std::array<int, 2> waiting;

	while(true)
	{
		if(died[prog])
		{
			prog ^= 1;
			if(died[prog])
				break;

			continue;
		}

		auto res = process(prog, ips[prog]);
		ips[prog] += res.first;

		// if we terminated, switch.
		if(ips[prog] >= input.size())
		{
			died[prog] = true;
			prog ^= 1;
		}

		// else if we yielded, switch.
		// if jmp is 0 and yield is true, means we did a recv and are waiting for a value
		else if(res.first == 0 && res.second)
		{
			waiting[prog] = true;
			prog ^= 1;
		}

		// if jmp is 1 and yield is true, means we just sent a value; unwait the other one.
		else if(res.first == 1 && res.second)
		{
			waiting[prog ^ 1] = false;
		}


		// check if the other program is also waiting
		if((waiting[0] || died[0]) && (waiting[1] || died[1]))
		{
			// stop
			break;
		}

		fprintf(stderr, "\r         %d", onesent);
	}

	// NO IDEA WHY
	printf("\nprog1 sent %d times\n", onesent / 2);


	#endif
}





