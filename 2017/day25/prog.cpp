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
#include <unordered_map>

int main()
{
	enum class State
	{
		A,
		B,
		C,
		D,
		E,
		F
	};

	int steps = 0;
	int target = 12586542;

	int cursor = 0;
	State state = State::A;
	std::unordered_map<int, int> tape;

	while(true)
	{
		if(steps == target)
		{
			int sum = 0;
			// count the thing.
			for(auto p : tape)
			{
				if(p.second)
					sum++;
			}

			printf("checksum = %d\n", sum);
			break;
		}


		switch(state)
		{
			case State::A: {
				if(tape[cursor] == 0)
				{
					tape[cursor] = 1;
					cursor++;
					state = State::B;
				}
				else
				{
					tape[cursor] = 0;
					cursor--;
					state = State::B;
				}
			} break;

			case State::B: {
				if(tape[cursor] == 0)
				{
					tape[cursor] = 0;
					cursor++;
					state = State::C;
				}
				else
				{
					tape[cursor] = 1;
					cursor--;
					state = State::B;
				}
			} break;

			case State::C: {
				if(tape[cursor] == 0)
				{
					tape[cursor] = 1;
					cursor++;
					state = State::D;
				}
				else
				{
					tape[cursor] = 0;
					cursor--;
					state = State::A;
				}
			} break;

			case State::D: {
				if(tape[cursor] == 0)
				{
					tape[cursor] = 1;
					cursor--;
					state = State::E;
				}
				else
				{
					tape[cursor] = 1;
					cursor--;
					state = State::F;
				}
			} break;

			case State::E: {
				if(tape[cursor] == 0)
				{
					tape[cursor] = 1;
					cursor--;
					state = State::A;
				}
				else
				{
					tape[cursor] = 0;
					cursor--;
					state = State::D;
				}
			} break;

			case State::F: {
				if(tape[cursor] == 0)
				{
					tape[cursor] = 1;
					cursor++;
					state = State::A;
				}
				else
				{
					tape[cursor] = 1;
					cursor--;
					state = State::E;
				}
			} break;
		}

		steps++;
	}
}





/*
Begin in state A.
Perform a diagnostic checksum after 12586542 steps.

In state A:
  If the current value is 0:
    - Write the value 1.
    - Move one slot to the right.
    - Continue with state B.
  If the current value is 1:
    - Write the value 0.
    - Move one slot to the left.
    - Continue with state B.

In state B:
  If the current value is 0:
    - Write the value 0.
    - Move one slot to the right.
    - Continue with state C.
  If the current value is 1:
    - Write the value 1.
    - Move one slot to the left.
    - Continue with state B.

In state C:
  If the current value is 0:
    - Write the value 1.
    - Move one slot to the right.
    - Continue with state D.
  If the current value is 1:
    - Write the value 0.
    - Move one slot to the left.
    - Continue with state A.

In state D:
  If the current value is 0:
    - Write the value 1.
    - Move one slot to the left.
    - Continue with state E.
  If the current value is 1:
    - Write the value 1.
    - Move one slot to the left.
    - Continue with state F.

In state E:
  If the current value is 0:
    - Write the value 1.
    - Move one slot to the left.
    - Continue with state A.
  If the current value is 1:
    - Write the value 0.
    - Move one slot to the left.
    - Continue with state D.

In state F:
  If the current value is 0:
    - Write the value 1.
    - Move one slot to the right.
    - Continue with state A.
  If the current value is 1:
    - Write the value 1.
    - Move one slot to the left.
    - Continue with state E.
*/










