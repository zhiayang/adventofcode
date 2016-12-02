// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 5

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


#define PART 2

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

	size_t numNice = 0;
	for(auto s : strings)
	{
		bool nice = false;

		#if PART == 1
		{
			// loop through chars
			int vowels = 0;
			bool illegal = false;
			bool hasdouble = 0;

			for(size_t i = 0; i < s.size(); i++)
			{
				if(s[i] == 'a' || s[i] == 'e' || s[i] == 'i' || s[i] == 'o' || s[i] == 'u')
					vowels++;

				// illegals: ab, cd, pq, xy
				if(i + 1 < s.size())
				{
					if(s[i] == 'a' && s[i + 1] == 'b')
						illegal = true;

					else if(s[i] == 'c' && s[i + 1] == 'd')
						illegal = true;

					else if(s[i] == 'p' && s[i + 1] == 'q')
						illegal = true;

					else if(s[i] == 'x' && s[i + 1] == 'y')
						illegal = true;


					if(s[i] == s[i + 1])
						hasdouble = true;

					if(illegal)
						break;
				}
			}

			nice = (vowels >= 3 && hasdouble && !illegal);
		}
		#elif PART == 2
		{
			bool hasbetween = false;			// axa
			bool hasrepeat = false;				// ab ... ab

			for(size_t i = 0; i < s.size(); i++)
			{
				if(i + 1 < s.size())
				{
					char find1 = s[i];
					char find2 = s[i + 1];

					// find in string
					for(size_t k = 0; k < s.size(); k++)
					{
						if(k + 1 < s.size())
						{
							if(k != i && k != i + 1 && k + 1 != i && k + 1 != i + 1
								&& s[k] == find1 && s[k + 1] == find2)
							{
								hasrepeat = true;
								break;
							}
						}
					}
				}
				if(i + 2 < s.size())
				{
					if(s[i] == s[i + 2]) hasbetween = true;
				}
			}

			nice = hasbetween && hasrepeat;
		}
		#else
			#error "???"
		#endif

		if(nice) numNice++;

		printf("%s -- %s\n", s.c_str(), nice ? "nice" : "naughty");
	}


	printf("%zu nice strings\n", numNice);

	fclose(f);
}







