// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 2

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <vector>
#include <algorithm>

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


	size_t totalPaper = 0;
	size_t totalRibbon = 0;
	std::vector<size_t> dims;

	for(size_t i = 0; i < size; i++)
	{
		size_t cnum = 0;

		while(buf[i] >= '0' && buf[i] <= '9')
		{
			cnum *= 10;
			cnum += buf[i] - '0';

			// printf("cnum = %zu (i = %zu // %c)\n", cnum, i, buf[i]);
			i++;
		}

		dims.push_back(cnum);

		// next letter is 'x'
		if(buf[i] == '\n')
		{
			// printf("%zu x %zu x %zu", dims[0], dims[1], dims[2]);
			size_t smallestArea = 1000000000;
			size_t smallestPeri = 1000000000;

			if(dims.size() != 3)
			{
				fprintf(stderr, "input error at pos %zu\n", i);
				exit(-1);
			}

			// we need WxH, WxL, LxH
			{
				size_t box = 0;

				size_t area = 0;
				size_t perimeter = 0;

				area = dims[0] * dims[1];
				if(area < smallestArea)
					smallestArea = area;

				box += 2 * area;



				area = dims[0] * dims[2];
				if(area < smallestArea)
					smallestArea = area;

				box += 2 * area;



				area = dims[1] * dims[2];
				if(area < smallestArea)
					smallestArea = area;

				box += 2 * area;

				// extra
				box += smallestArea;
				// printf(" box = %zu\n", box);
				totalPaper += box;





				std::sort(dims.begin(), dims.end());
				totalRibbon += 2 * (dims[0] + dims[1]);
				totalRibbon += dims[0] * dims[1] * dims[2];
			}






			dims.clear();
			cnum = 0;
			continue;
		}
		else if(i == size || buf[i] == 0)
		{
			break;
		}
		else if(buf[i] != 'x')
		{
			fprintf(stderr, "input error: char %zu (%c // %d)\n", i, buf[i], buf[i]);
			exit(-1);
		}
	}

	printf("paper = %zu, ribbon = %zu\n", totalPaper, totalRibbon);

	fclose(f);
}





















