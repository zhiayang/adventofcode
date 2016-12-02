// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 17

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <map>
#include <deque>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>

FILE* openInput(size_t* size, uint8_t** buf)
{
	FILE* f = fopen("input", "r");
	if(f == 0)
	{
		fprintf(stderr, "file read error FILE* was null :: %d / %s\n", errno, strerror(errno));
		exit(-1);
	}

	struct stat st;
	fstat(fileno(f), &st);

	*size = st.st_size;
	*buf = new uint8_t[*size + 1];
	if(*buf == 0)
	{
		fprintf(stderr, "failed to allocate %zu bytes\n", *size + 1);
		exit(-1);
	}

	size_t readbytes = fread(*buf, 1, *size, f);
	if(readbytes != *size)
	{
		fprintf(stderr, "file read error (need %zu bytes, read %zu) :: %d / %s\n", *size, readbytes, errno, strerror(errno));
		exit(-1);
	}

	return f;
}


int main()
{
	size_t size = 0;
	uint8_t* buf = 0;

	FILE* f = openInput(&size, &buf);


	std::vector<std::string> strings;
	std::string cur = "";

	for(size_t i = 0; i < size; i++)
	{
		if(buf[i] != '\n' && buf[i] != 0)
		{
			cur += buf[i];
		}
		else
		{
			strings.push_back(cur);
			cur = "";
		}
	}



	std::vector<int> containers;
	for(auto l : strings)
	{
		containers.push_back(std::stoi(l));
	}



	// there are 2^N possible combinations
	// every number from 0 to 2^N uniquely identifies each possible combination,
	// if we take a 0 bit as not using the container, and a 1 bit as using the container.
	// hence, get every number from 1 to 2^N, then loop through, shifting it, and checking it.

	size_t num = 0;
	for(size_t i = 0; i < (1 << containers.size()); i++)
	{
		// std::cout << std::bitset<20>(i) << "\n";
		size_t t = i;
		size_t s = 0;

		for(int j : containers)
		{
			if(t & 0x1)
				s += j;

			t >>= 1;

			if(s > 150)
				break;
		}

		if(s == 150)
			num++;
	}

	printf("num = %zu\n", num);







	fclose(f);
}


























