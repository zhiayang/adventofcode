// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 8

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


	std::vector<std::string> lines;
	std::vector<std::string> escapedlines;
	std::vector<std::string> encodedlines;
	{
		std::string cur = "";
		for(size_t i = 0; i < size; i++)
		{
			if(buf[i] != '\n' && buf[i] != 0)
			{
				cur += buf[i];
			}
			else
			{
				lines.push_back(cur);
				cur = "";
			}
		}
	}



	for(auto str : lines)
	{
		std::string escaped = "";
		for(size_t i = 0; i < str.length(); i++)
		{
			if(str[i] == '\\')
			{
				if(i + 1 < str.size() && str[i + 1] == '\\')
				{
					escaped += "\\";
					i++;
				}
				else if(i + 1 < str.size() && str[i + 1] == '"')
				{
					escaped += "\"";
					i++;
				}
				else if(i + 3 < str.size() && str[i + 1] == 'x')
				{
					std::string hex = "";
					hex += str[i + 2];
					hex += str[i + 3];

					size_t last = 0;
					uint8_t hx = std::stol(hex, &last, 16);

					escaped += "+";
					i += 3;
				}
			}
			else
			{
				escaped += str[i];
			}
		}

		// do encoding
		std::string encoded;
		for(size_t i = 0; i < str.length(); i++)
		{
			if(str[i] == '"')
			{
				encoded += "\\\"";
			}
			else if(str[i] == '\\')
			{
				encoded += "\\\\";
			}
			else
			{
				encoded += str[i];
			}
		}

		encoded = '"' + encoded + '"';
		escapedlines.push_back(escaped);
		encodedlines.push_back(encoded);

		size_t d1 = encoded.length() - str.length();
		size_t d2 = 2 + std::count(str.begin(), str.end(), '\\') + std::count(str.begin(), str.end(), '"');

		printf("%02zu: %54s -> %02zu: %54s -> %02zu :: %zu :: [%zu]\n", str.length(), str.c_str(), encoded.length(), encoded.c_str(),
			d1, d2, d1 - d2);

		if(d1 != d2)
			abort();
	}


	size_t encodedBytes = 0;
	size_t textualBytes = 0;
	size_t memoryBytes = 0;

	for(size_t i = 0; i < escapedlines.size(); i++)
	{
		encodedBytes += encodedlines[i].length();
		textualBytes += lines[i].length();	// plus opening and closing quotes
		memoryBytes += escapedlines[i].length();
	}

	printf("textual = %zu, memory = %zu, encoded = %zu\n", textualBytes, memoryBytes, encodedBytes);


	fclose(f);
}







