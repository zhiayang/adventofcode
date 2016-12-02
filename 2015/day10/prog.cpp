// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 10

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>


static int curIterations = 0;
static int numIterations = 50;		// part 1 = 40, part 2 = 50

std::string iterate(std::string input);

int main()
{
	std::string input = "1113122113";
	std::string out = iterate(input);

	printf("output length = %zu\noutput = %s\n", out.length(), out.c_str());
}

std::string iterate(std::string input)
{
	if(curIterations == numIterations)
		return input;

	size_t count = 0;
	char c = 0;

	std::string out = "";

	for(size_t i = 0; i < input.length(); i++)
	{
		if(c == 0 || c == input[i])
		{
			count++;
			c = input[i];
		}
		else
		{
			out += std::to_string(count) + c;

			count = 1;
			c = input[i];
		}
	}

	out += std::to_string(count) + c;

	curIterations++;
	printf("iterated %d times\n", curIterations);

	return iterate(out);
}










