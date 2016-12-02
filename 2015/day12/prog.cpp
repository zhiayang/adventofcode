// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 12

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

#include "json.hpp"
using json = nlohmann::json;

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


static bool skipred = true;	// true for part2

template <typename T>
void doSum(const T& objs, ssize_t* cur)
{
	for(auto e : objs)
	{
		if(e.is_array())
		{
			doSum(e, cur);
		}
		else if(e.is_object())
		{
			bool red = false;
			for(json::iterator it = e.begin(); it != e.end(); it++)
			{
				if(it.value() == "red")
				{
					red = true;
					break;
				}
			}

			if(!red && skipred)
				doSum(e, cur);
		}
		else if(e.is_number())
		{
			*cur += e.template get<ssize_t>();
		}
	}
}

int main()
{
	size_t size = 0;
	uint8_t* buf = 0;

	FILE* f = openInput(&size, &buf);


	std::string input = (char*) buf;
	json objs = json::parse(input);


	ssize_t sum = 0;
	for(auto e : objs)
	{
		// std::cout << e << "\n";
		if(e.is_number())
		{
			ssize_t n = e.get<ssize_t>();
			sum += n;
		}
		else if(e.is_array())
		{
			doSum(e, &sum);
		}
		else if(e.is_object())
		{
			bool red = false;
			for(json::iterator it = e.begin(); it != e.end(); it++)
			{
				if(it.value() == "red")
				{
					red = true;
					break;
				}
			}

			if(!red && skipred)
				doSum(e, &sum);
		}
	}

	printf("total sum = %zu\n", sum);

	fclose(f);
}



















