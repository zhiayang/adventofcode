// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 11

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


static int n = 0;
std::string next(std::string& cur)
{
	// fprintf(stderr, "\r                 \r%d", n++);

	std::string& ret = cur;
	for(size_t i = ret.length() - 1; i > 0; i--)
	{
		if(ret[i] != 'z')
		{
			ret[i]++;
			break;
		}
		else
		{
			ret[i] = 'a';
		}
	}


	// illegal chars
	if(ret.find('i') != std::string::npos || ret.find('o') != std::string::npos || ret.find('l') != std::string::npos)
		return next(ret);



	// check for two pairs of letters

	char c = ret[0];
	int pairs = 0;
	for(size_t i = 0; i < ret.length() && pairs < 2; i++)
	{
		if(i + 1 < ret.length())
		{
			if(ret[i + 1] == c)
				pairs++, i++;		// skip it (avoid overlaps, eg. ccc from registering as 2 pairs)


			c = ret[i + 1];
		}
	}

	if(pairs < 2)
		return next(ret);

	bool pass = false;
	for(size_t i = 0; i < ret.length() && !pass; i++)
	{
		if(i + 2 < ret.length())
		{
			if(ret[i] + 1 == ret[i + 1] && ret[i] + 2 == ret[i + 2])
				pass = true;
		}
	}

	if(!pass)
		return next(ret);

	return ret;
}




int main()
{
	std::string input = "hepxcrrq";
	std::string output = next(input);
	output = next(output); // part 2

	printf("new password = %s\n", output.c_str());
}











