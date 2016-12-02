// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 3

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <map>
#include <vector>
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



	// coords are stored as 32-bit pairs, in a 64-bit int
	// x is higher 32-bit, y is lower 32-bit.

	std::map<std::pair<int, int>, int> houses;
	std::map<std::pair<int, int>, int> orighouses;


	houses[{ 0, 0 }] = 1; // deliver to starting house

	int32_t santax = 0;
	int32_t santay = 0;

	int32_t robox = 0;
	int32_t roboy = 0;

	int32_t origx = 0;
	int32_t origy = 0;

	for(size_t i = 0; i < size; i++)
	{
		if(buf[i] == '^')
		{
			i % 2 == 0 ? santay++ : roboy++;
			origy++;
		}

		else if(buf[i] == 'v')
		{
			i % 2 == 0 ? santay-- : roboy--;
			origy--;
		}

		else if(buf[i] == '<')
		{
			i % 2 == 0 ? santax-- : robox--;
			origx--;
		}

		else if(buf[i] == '>')
		{
			i % 2 == 0 ? santax++ : robox++;
			origx++;
		}

		else if(buf[i] == 0 || buf[i] == '\n')
			break;

		else
		{
			fprintf(stderr, "input error at char %zu: %c\n", i, buf[i]);
			exit(-1);
		}

		houses[{ i % 2 == 0 ? santax : robox, i % 2 == 0 ? santay : roboy }] += 1;
		orighouses[{ origx, origy }] += 1;
	}

	printf("total houses = %zu // original = %zu\n", houses.size(), orighouses.size());







	fclose(f);
}







