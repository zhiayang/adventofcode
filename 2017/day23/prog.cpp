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
	// std::vector<std::string_view> input = {
	// 	/* 0 */  "set b 65",
	// 	/* 1 */  "set c b",
	// 	/* 2 */  "jnz a 2",
	// 	/* 3 */  "jnz 1 5",
	// 	/* 4 */  "mul b 100",
	// 	/* 5 */  "sub b -100000",
	// 	/* 6 */  "set c b",
	// 	/* 7 */  "sub c -17000",
	// 	/* 8 */  "set f 1",
	// 	/* 9 */  "set d 2",
	// 	/* 10 */ "set e 2",
	// 	/* 11 */ "set g d",
	// 	/* 12 */ "mul g e",
	// 	/* 13 */ "sub g b",
	// 	/* 14 */ "jnz g 2",
	// 	/* 15 */ "set f 0",
	// 	/* 16 */ "sub e -1",
	// 	/* 17 */ "set g e",
	// 	/* 18 */ "sub g b",
	// 	/* 19 */ "jnz g -8",
	// 	/* 20 */ "sub d -1",
	// 	/* 21 */ "set g d",
	// 	/* 22 */ "sub g b",
	// 	/* 23 */ "jnz g -13",
	// 	/* 24 */ "jnz f 2",
	// 	/* 25 */ "sub h -1",
	// 	/* 26 */ "set g b",
	// 	/* 27 */ "sub g c",
	// 	/* 28 */ "jnz g 2",
	// 	/* 29 */ "jnz 1 3",
	// 	/* 30 */ "sub b -17",
	// 	/* 31 */ "jnz 1 -23"
	// };

	std::vector<std::string_view> input = {
		/* 0 */  "set b 65",
		/* 1 */  "set c b",
		/* 2 */  "jnz a 2",
		/* 3 */  "jnz 1 5",
		/* 4 */  "mul b 100",
		/* 5 */  "sub b -100000",
		/* 6 */  "set c b",
		/* 7 */  "sub c -17000",

		/* 8 */  "set f 1",
		/* 9 */  "set d 2",
		/* 10 */ "set e 2",

		/* 11 */ "set g d",
		/* 12 */ "mul g e",
		/* 13 */ "sub g b",

		/* 14 */ "jnz g 2",

		/* 15 */ "set f 0",

		/* 16 */ "sub e -1",

		/* 17 */ "set g e",

		/* 18 */ "sub g b",

		/* 19 */ "jnz g -8",

		/* 20 */ "sub d -1",

		/* 21 */ "set g d",
		/* 22 */ "sub g b",
		/* 23 */ "jnz g -13",

		/* 24 */ "jnz f 2",

		/* 25 */ "sub h -1",

		/* 26 */ "set g b",
		/* 27 */ "sub g c",
		/* 28 */ "jnz g 2",
		/* 29 */ "jnz 1 3",
		/* 30 */ "sub b -17",
		/* 31 */ "jnz 1 -23"
	};

	#if 0

	int mulcnt = 0;
	std::array<int64_t, 26> regs;

	for(int i = 0; i < input.size(); )
	{
		auto instr = input[i];

		fprintf(stderr, "a = %lld ", regs['a' - 'a']);
		fprintf(stderr, "b = %lld ", regs['b' - 'a']);
		fprintf(stderr, "c = %lld ", regs['c' - 'a']);
		fprintf(stderr, "d = %lld ", regs['d' - 'a']);
		fprintf(stderr, "e = %lld ", regs['e' - 'a']);
		fprintf(stderr, "f = %lld ", regs['f' - 'a']);
		fprintf(stderr, "g = %lld ", regs['g' - 'a']);
		fprintf(stderr, "h = %lld ", regs['h' - 'a']);
		printf("\n");

		if(instr.find("sub") == 0)
		{
			instr.remove_prefix(4);
			char reg = instr[0];

			instr.remove_prefix(2);
			int val = 0;
			if(isalpha(instr[0]))
				val = regs[instr[0] - 'a'];

			else
				val = std::stoi(std::string(instr.data(), instr.length()));

			regs[reg - 'a'] -= val;
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
			mulcnt++;

			// printf("%c *= %d == %d\n", reg, val, regs[reg - 'a']);
		}
		else if(instr.find("jnz") == 0)
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
			if(cond != 0)
			{
				i += jmp;
				continue;
			}
		}
		i++;
	}

	printf("mulcnt = %d\n", mulcnt);
	printf("h = %lld\n", regs['h' - 'a']);

	#else

	/*

		the innermost loop appears to be doing some kind of modulo operation
		it's some kind of factor problem.

		i think it's finding non-prime numbers between 'b' (106500) and 'c' (123500).
		h is incremented when f is 0, and f is 0 when some factors are found (d * e == b)

		regardless, b is then += 17 and jumps all the way to the start

		so it's range(b, c) with a step of 17, that are not prime.
	*/

	auto isprime = [](int x) -> bool {
		for(int k = 2; k * k < x; k++)
			if(x % k == 0) return false;

		return true;
	};

	int h = 0;
	for(int i = 106500; i <= 123500; i += 17)
	{
		if(!isprime(i)) h++;
	}

	printf("h = %d\n", h);

	#endif
}





