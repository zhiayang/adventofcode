// prog.cpp
// Copyright (c) 2014 - 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <set>
#include <map>
#include <array>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include <algorithm>

int main()
{
	const auto input = 363;

	#if 0

	size_t position = 0;
	std::vector<int> circle;

	for(int i = 0; i < 2017 + 1; i++)
	{
		circle.insert(circle.begin() + position, i);
		position += input;
		position %= circle.size();

		// make a slot after the current position, and make it the new thing
		position++;

		fprintf(stderr, "\r           \r%d", i);
	}

	for(auto i = 0; i < circle.size(); i++)
	{
		if(circle[i] == 2017)
		{
			printf("\n");
			for(auto k = i; k < circle.size(); k++)
				printf("%d ", circle[k]);

			printf("\n");

			break;
		}
	}
	#else


	size_t size = 1;
	size_t position = 0;

	int num = 0;

	// everything is fake. zero is always at the front,
	// so we just bother about when position == 1, and nothing else.
	for(int i = 1; i < 50e6 + 1; i++)
	{
		position += input;
		position %= size;

		// make a slot after the current position, and make it the new thing
		position++;

		if(position == 1)
			num = i;

		size++;
	}

	printf("i = %d\n", num);
	#endif
}




