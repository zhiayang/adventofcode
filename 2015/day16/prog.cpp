// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 16

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







struct ASue
{
	int num = 0;

	int children = -1;
	int cats = -1;

	int samoyeds = -1;
	int pomeranians = -1;
	int akitas = -1;
	int vizslas = -1;

	int goldfish = -1;
	int trees = -1;
	int cars = -1;
	int perfumes = -1;
};


bool match(int param, int given)
{
	return (given == -1 || given == param);
}

bool matchGT(int param, int given)
{
	return (given == -1 || given > param);
}

bool matchLT(int param, int given)
{
	return (given == -1 || given < param);
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




	std::vector<ASue> sues;
	sues.reserve(500);

	for(auto s : strings)
	{
		std::string str = s.substr(strlen("Sue" ));

		size_t i = 0;
		int nsue = std::stoi(str, &i);

		str = str.substr(i);
		// printf("s = %s\n", str.c_str());

		// remove colon
		str = str.substr(strlen(": "));

		ASue sue;
		sue.num = nsue;

		while(str.length() > 0)
		{
			// printf("s = %s\n", str.c_str());
			if(str.find("children") == 0)
			{
				size_t n = 0;
				str = str.substr(strlen("children: "));
				sue.children = std::stoi(str, &n);

				str = str.substr(n);

				if(str.length() > 0)
					str = str.substr(strlen(", "));
			}
			else if(str.find("cats") == 0)
			{
				size_t n = 0;
				str = str.substr(strlen("cats: "));
				sue.cats = std::stoi(str, &n);

				str = str.substr(n);

				if(str.length() > 0)
					str = str.substr(strlen(", "));
			}
			else if(str.find("samoyeds") == 0)
			{
				size_t n = 0;
				str = str.substr(strlen("samoyeds: "));
				sue.samoyeds = std::stoi(str, &n);

				str = str.substr(n);

				if(str.length() > 0)
					str = str.substr(strlen(", "));
			}
			else if(str.find("pomeranians") == 0)
			{
				size_t n = 0;
				str = str.substr(strlen("pomeranians: "));
				sue.pomeranians = std::stoi(str, &n);

				str = str.substr(n);

				if(str.length() > 0)
					str = str.substr(strlen(", "));
			}
			else if(str.find("akitas") == 0)
			{
				size_t n = 0;
				str = str.substr(strlen("akitas: "));
				sue.akitas = std::stoi(str, &n);

				str = str.substr(n);

				if(str.length() > 0)
					str = str.substr(strlen(", "));
			}
			else if(str.find("vizslas") == 0)
			{
				size_t n = 0;
				str = str.substr(strlen("vizslas: "));
				sue.vizslas = std::stoi(str, &n);

				str = str.substr(n);

				if(str.length() > 0)
					str = str.substr(strlen(", "));
			}
			else if(str.find("goldfish") == 0)
			{
				size_t n = 0;
				str = str.substr(strlen("goldfish: "));
				sue.goldfish = std::stoi(str, &n);

				str = str.substr(n);

				if(str.length() > 0)
					str = str.substr(strlen(", "));
			}
			else if(str.find("trees") == 0)
			{
				size_t n = 0;
				str = str.substr(strlen("trees: "));
				sue.trees = std::stoi(str, &n);

				str = str.substr(n);

				if(str.length() > 0)
					str = str.substr(strlen(", "));
			}
			else if(str.find("cars") == 0)
			{
				size_t n = 0;
				str = str.substr(strlen("cars: "));
				sue.cars = std::stoi(str, &n);

				str = str.substr(n);

				if(str.length() > 0)
					str = str.substr(strlen(", "));
			}
			else if(str.find("perfumes") == 0)
			{
				size_t n = 0;
				str = str.substr(strlen("perfumes: "));
				sue.perfumes = std::stoi(str, &n);

				str = str.substr(n);

				if(str.length() > 0)
					str = str.substr(strlen(", "));
			}
		}

		sues.push_back(sue);
	}





	int children = 3;
	int cats = 7;
	int samoyeds = 2;
	int pomeranians = 3;
	int akitas = 0;
	int vizslas = 0;
	int goldfish = 5;
	int trees = 3;
	int cars = 2;
	int perfumes = 1;

	std::vector<ASue> matches;
	for(auto sue : sues)
	{
		// part 1
		#if 0
		if(match(children, sue.children) && match(cats, sue.cats) && match(samoyeds, sue.samoyeds)
			&& match(pomeranians, sue.pomeranians) && match(akitas, sue.akitas) && match(vizslas, sue.vizslas)
			&& match(goldfish, sue.goldfish) && match(trees, sue.trees) && match(cars, sue.cars) && match(perfumes, sue.perfumes))
		{
			matches.push_back(sue);
		}
		#endif


		// part 2
		if(match(children, sue.children) && matchGT(cats, sue.cats) && match(samoyeds, sue.samoyeds)
			&& matchLT(pomeranians, sue.pomeranians) && match(akitas, sue.akitas) && match(vizslas, sue.vizslas)
			&& matchLT(goldfish, sue.goldfish) && matchGT(trees, sue.trees) && match(cars, sue.cars) && match(perfumes, sue.perfumes))
		{
			matches.push_back(sue);
		}
	}

	printf("%zu full matches\n", matches.size());

	for(auto sue : matches)
	{
		printf("%d\n{\n"
				"\tchildren: %d\n"
				"\tcats: %d\n"
				"\tsamoyeds: %d\n"
				"\tpomeranians: %d\n"
				"\takitas: %d\n"
				"\tvizslas: %d\n"
				"\tgoldfish: %d\n"
				"\ttrees: %d\n"
				"\tcars: %d\n"
				"\tperfumes: %d\n}\n\n", sue.num, sue.children, sue.cats, sue.samoyeds, sue.pomeranians, sue.akitas, sue.vizslas,
				sue.goldfish, sue.trees, sue.cars, sue.perfumes);
	}


	fclose(f);
}


























