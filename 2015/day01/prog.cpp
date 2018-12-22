// prog.cpp
// Copyright (c) 2018, zhiayang
// Licensed under the Apache License Version 2.0.

#include "assert.h"

#include "utils.h"
#include "tinyformat.h"

int main()
{
	auto input = util::readFile("input.txt");

	int basement_index = -1;
	int cur_floor = 0;
	for(size_t i = 0; i < input.size(); i++)
	{
		if(input[i] == '(') cur_floor++;
		else                cur_floor--;

		if(cur_floor == -1 && basement_index == -1)
			basement_index = i + 1;
	}

	tfm::printfln("part 1: final floor = %d", cur_floor);
	tfm::printfln("part 2: basement index = %d", basement_index);
}
