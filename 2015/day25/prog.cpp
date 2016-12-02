// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 25

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




size_t transform(size_t num)
{
	size_t ret = num * 252533;
	return ret % 33554393;
}

int main()
{
	size_t targetX = 3029;
	size_t targetY = 2947;

	// the number of times we need to apply the sequence
	size_t seqNum = 0;


	for(size_t x = 1; x <= targetX; x++)
		seqNum += x;

	for(size_t y = 0; y < targetY - 1; y++)
		seqNum += (targetX + y);


	printf("applying process %zu times\n", seqNum);



	size_t num = 20151125;
	for(size_t i = 0; i < seqNum - 1; i++)
		num = transform(num);

	printf("output = %zu\n", num);
}







