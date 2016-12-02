// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 6

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <map>
#include <vector>
#include <bitset>
#include <string>
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

size_t tonum(std::string s, size_t* index)
{
	size_t ret = 0;
	try
	{
		ret = std::stoi(s, index);
	}
	catch(std::exception)
	{
	}

	return ret;
}



#define PART 2

int main()
{
	size_t size = 0;
	uint8_t* buf = 0;

	FILE* f = openInput(&size, &buf);


	#if PART == 1
		std::bitset<1000 * 1000> lights;
	#elif PART == 2
		std::vector<size_t> lights;
		for(size_t i = 0; i < 1000 * 1000; i++)
			lights.push_back(0);

	#else
		#error "???"
	#endif



	std::vector<std::string> instrs;
	std::string cur = "";
	for(size_t i = 0; i < size; i++)
	{
		if(buf[i] != '\n' && buf[i] != 0)
		{
			cur += buf[i];
		}
		else
		{
			instrs.push_back(cur);
			cur = "";
		}
	}



	for(auto instr : instrs)
	{
		int action = 0; // on = 1, off = 2, toggle = 3
		size_t x1 = 0, y1 = 0;
		size_t x2 = 0, y2 = 0;

		{
			if(instr.find("turn on ") == 0)
			{
				action = 1;
				instr = instr.substr(strlen("turn on "));
			}
			else if(instr.find("turn off ") == 0)
			{
				action = 2;
				instr = instr.substr(strlen("turn off "));
			}
			else if(instr.find("toggle ") == 0)
			{
				action = 3;
				instr = instr.substr(strlen("toggle "));
			}
			else
			{
				fprintf(stderr, "input error: %s\n", instr.c_str());
				exit(-1);
			}







			// parse the first number.

			size_t last = 0;
			x1 = tonum(instr, &last);

			if(instr[last] != ',')
			{
				fprintf(stderr, "input error: %s\n", instr.c_str());
				exit(-1);
			}

			instr = instr.substr(last + 1);		// plus comma
			y1 = tonum(instr, &last);

			instr = instr.substr(last);
			if(instr.find(" through ") != 0)
			{
				fprintf(stderr, "input error: %s\n", instr.c_str());
				exit(-1);
			}


			instr = instr.substr(strlen(" through "));


			x2 = tonum(instr, &last);

			if(instr[last] != ',')
			{
				fprintf(stderr, "input error: %s\n", instr.c_str());
				exit(-1);
			}

			instr = instr.substr(last + 1);		// plus comma
			y2 = tonum(instr, &last);
		}


		for(size_t y = y1; y <= y2; y++)
		{
			for(size_t x = x1; x <= x2; x++)
			{
				#if PART == 1
				{
					if(action == 1)
						lights.set(y * 1000 + x);

					else if(action == 2)
						lights.reset(y * 1000 + x);

					else if(action == 3)
						lights.flip(y * 1000 + x);
				}
				#elif PART == 2
				{
					if(action == 1)
						lights[y * 1000 + x] += 1;

					else if(action == 2 && lights[y * 1000 + x] > 0)
						lights[y * 1000 + x] -= 1;

					else if(action == 3)
						lights[y * 1000 + x] += 2;
				}
				#else
				{
					#error "???"
				}
				#endif
			}
		}
	}



	#if PART == 1
	{
		printf("%zu lights on\n", lights.count());
	}
	#elif PART == 2
	{
		size_t total = 0;
		for(auto light : lights)
			total += light;

		printf("total brightness = %zu\n", total);
	}
	#else
	{
		#error "???"
	}
	#endif






	fclose(f);
}







