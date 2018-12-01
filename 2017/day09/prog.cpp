// prog.cpp
// Copyright (c) 2014 - 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <map>
#include <array>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "lexer.cpp"

int main()
{
	auto input = readEntireFile("day09/input.txt");
	// std::string_view input = "{{<a!>},{<a!>},{<a!>},{<ab>}}";

	int groupSum = 0;

	int rubbish = 0;
	int groupNest = 0;
	bool garbage = false;
	while(!input.empty())
	{
		if(garbage && input[0] != '>' && input[0] != '!')
		{
			rubbish++;
			input.remove_prefix(1);
			continue;
		}

		if(input[0] == '!')
		{
			input.remove_prefix(2);
			continue;
		}

		if(input[0] == '{')
			groupNest++;

		else if(input[0] == '<')
			garbage = true;

		else if(input[0] == '}')
			groupSum += groupNest, groupNest--;

		else if(input[0] == '>')
			garbage = false;

		else if(input[0] == ',')
			;

		input.remove_prefix(1);
	}

	printf("sum = %d\n", groupSum);
	printf("rubbish = %d\n", rubbish);
}










