#if 1
// prog.cpp
// Copyright (c) 2014 - 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include "assert.h"

#include <map>
#include <set>
#include <list>
#include <stack>
#include <array>
#include <deque>
#include <string>
#include <vector>
#include <fstream>
#include <functional>
#include <unordered_set>

#include "utils.h"
#include "tinyformat.h"



struct water_t
{
	water_t(int x, int y, bool f) : x(x), y(y), flowing(f) { }

	int x;
	int y;

	bool flowing;
};

int maxx = 0;
int maxy = 0;

char& get(char* map, int x, int y)
{
	return map[x + y*maxx];
}

std::string print(char* map, int cx, int cy)
{
	std::string ret;
	for(int y = 0; y < maxy; y++)
	{
		for(int x = 0; x < maxx; x++)
		{
			if(x == cx && y == cy)
			{
				ret += tfm::format("%s%s%s", COLOUR_RED, map[x + y*maxx], COLOUR_RESET);
			}
			else
			{
				ret += map[x + y*maxx];
			}
		}
		ret += "\n";
	}

	ret += "______________________________________\n";
	return ret;
}

int main()
{
	auto output = std::ofstream("day17/output.txt", std::ios::out);

	std::vector<std::string> lines;
	{
		auto input = std::ifstream("day17/input.txt", std::ios::in);
		for(std::string line; std::getline(input, line); )
			lines.push_back(line);
	}

	std::vector<std::pair<int, int>> clays;

	for(const auto& line : lines)
	{
		if(line == "\n") continue;

		if(line[0] == 'x')
		{
			int x, y0, y1;
			sscanf(line.c_str(), "x=%d, y=%d..%d", &x, &y0, &y1);
			assert(y0 <= y1);

			for(int y = y0; y <= y1; y++)
				clays.push_back({ x, y });
		}
		else
		{
			int y, x0, x1;
			sscanf(line.c_str(), "y=%d, x=%d..%d", &y, &x0, &x1);
			assert(x0 <= x1);

			for(int x = x0; x <= x1; x++)
				clays.push_back({ x, y });
		}
	}

	// set up x from 0 to 2000.
	// y from 0 to the maximum y

	maxx = 2000;
	maxy = INT_MIN;

	int miny = INT_MAX;

	for(auto [ _, y ] : clays)
	{
		if(y > maxy)      maxy = y;
		else if(y < miny) miny = y;
	}

	maxy += 1;
	char* map = new char[maxx * maxy];
	memset(map, ' ', maxx * maxy);
	tfm::printfln("map size: %d x %d", maxx, maxy);

	for(auto& c : clays)
		map[c.first + (maxx * c.second)] = '#';

	std::deque<water_t> waters;
	waters.push_back(water_t(500, 0, true));
	get(map, 500, 0) = '+';

	while(waters.size() > 0)
	{
		auto& water = waters.front();
		waters.pop_front();

		int x = water.x;
		int y = water.y;

		// check if we're past the limit.
		if(!water.flowing) continue;
		if(y >= maxy || y + 1 >= maxy) continue;


		// ok we're not. see if there's something below.
		auto& cur = get(map, x, y);
		auto& down = get(map, x, y + 1);
		if(down == '|') continue;


		if(down == ' ')
		{
			// spread down!
			down = '|';
			waters.push_front(water_t(x, y + 1, true));
		}
		else if(down == '#' || down == '~')
		{
			// spread sideways.
			bool lblocked = false;
			bool rblocked = false;

			if(x > 0)
			{
				auto& l = get(map, x - 1, y);
				if(l == ' ')
				{
					assert(l == ' ');

					l = '-';
					waters.push_front(water_t(x - 1, y, true));
				}
				else
				{
					lblocked = true;
				}
			}
			if(x < maxx)
			{
				auto& r = get(map, x + 1, y);
				if(r == ' ')
				{
					assert(r == ' ');

					r = '-';
					waters.push_front(water_t(x + 1, y, true));
				}
				else
				{
					rblocked = true;
				}
			}


			if(lblocked && rblocked)
			{
				auto& l = get(map, x - 1, y);
				auto& r = get(map, x + 1, y);

				auto& u = get(map, x, y - 1);

				if(l == '#' && r == '#')
				{
					cur = '~';
					water.flowing = false;
				}
				else
				{
					bool leftwall = false;
					bool supported = false;

					// see which side is a wall
					if(l == '#' || l == '~')
					{
						leftwall = true;

						// scan right
						for(int k = x; k < maxx; k++)
						{
							auto c = get(map, k, y);
							auto d = get(map, k, y + 1);

							if((c == '-' || c == '|') && (d == '~' || d == '#'))
								continue;

							if((c == '#' || c == '~') && (d == '~' || d == '#'))
							{
								supported = true;
								break;
							}

							break;
						}

					}
					else if(r == '#' || r == '~')
					{
						// scan left
						for(int k = x; k > 0; k--)
						{
							auto c = get(map, k, y);
							auto d = get(map, k, y + 1);

							if((c == '-' || c == '|') && (d == '~' || d == '#'))
								continue;

							if((c == '#' || c == '~') && (d == '~' || d == '#'))
							{
								supported = true;
								break;
							}

							break;
						}
					}


					if(supported)
					{
						cur = '~';
						water.flowing = false;

						if(leftwall && (get(map, x + 1, y) == '-' || get(map, x + 1, y) == '|'))
							waters.push_back(water_t(x + 1, y, true));

						else if(get(map, x - 1, y) == '-' || get(map, x - 1, y) == '|')
							waters.push_back(water_t(x - 1, y, true));

						if(get(map, x, y - 1) == '|')
							waters.push_back(water_t(x, y - 1, true));
					}
				}
			}
		}
		else if(down == '|' || down == '-')
		{
			// if we're still flowing down onto flowing water, then that flowing water becomes still water.
			down = '~';
			waters.push_front(water_t(x, y + 1, false));
		}
		else
		{
			tfm::printfln("down = '%c'!", down);
			assert(!"should not happen!");
		}


		// tfm::printfln("%s\n", print(map, x, y));
		auto l = print(map, x, y);
		std::replace(l.begin(), l.end(), ' ', '.');
		output << l;
	}

	output.close();


	// count the waters?!
	int water_count = 0;
	for(int y = 0; y < maxy; y++)
	{
		for(int x = 0; x < maxx; x++)
		{
			auto c = get(map, x, y);
			if(util::match(c, '~', '|', '-'))
				water_count++;
		}
	}

	tfm::printfln("part 1: %d tiles reachable by water", water_count);
}




#else



#include <stdio.h>
#define SIZE 2000

int main()
{
  char First, Second, Line[100], Area[SIZE][SIZE];
  int Steady, RangeMin, RangeMax, XMin, XMax, YMin, YMax, Count, InnerCount;
  int SideCount, LeftBound, RightBound, TotalArea;
  bool Changed, LeftBounded, RightBounded;
  for (Count=0;Count<SIZE;Count++)
    for (InnerCount=0;InnerCount<SIZE;InnerCount++)
      Area[Count][InnerCount] = '.';
  XMin=XMax=464;
  YMin=YMax=310;
  while (fgets(Line, 90, stdin))
    {
      sscanf(Line, "%c=%d, %c=%d..%d\n", &First, &Steady, &Second, &RangeMin, &RangeMax);
      if (Second=='x')
    {
      for (Count=RangeMin;Count<=RangeMax;Count++)
        Area[Steady][Count] = '#';
      if (RangeMin<XMin)
        XMin=RangeMin;
      if (RangeMin>XMax)
        XMax=RangeMin;
      if (RangeMax<XMin)
        XMin=RangeMin;
      if (RangeMax>XMax)
        XMax=RangeMin;
      if (Steady<YMin)
        YMin=Steady;
      if (Steady>YMax)
        YMax=Steady;
    }
      else
    {
      for (Count=RangeMin;Count<=RangeMax;Count++)
        Area[Count][Steady] = '#';
      if (RangeMin<YMin)
        YMin=RangeMin;
      if (RangeMin>YMax)
        YMax=RangeMin;
      if (RangeMax<YMin)
        YMin=RangeMin;
      if (RangeMax>YMax)
        YMax=RangeMin;
      if (Steady<XMin)
        XMin=Steady;
      if (Steady>XMax)
        XMax=Steady;
    }
    }
  printf("X: %d %d Y: %d %d\n", XMin, XMax, YMin, YMax);
  Changed = true;
  Area[0][500] = '|';
  printf("%c\n", Area[0][500]);
  for (Count=0;Count<=YMax;Count++)
    {
      for (InnerCount=(XMin-2);InnerCount<=(XMax+2);InnerCount++)
    printf("%c", Area[Count][InnerCount]);
      printf("\n");
    }
  while (Changed)
    {
      Changed = false;
      for (Count=0;Count<=YMax;Count++)
    for (InnerCount=(XMin-2);InnerCount<=(XMax+2);InnerCount++)
      {
        if (Area[Count][InnerCount]=='|')
          {
        if (Area[Count+1][InnerCount] == '.')
          {
            Area[Count+1][InnerCount] = '|';
            Changed=true;
          }
        else if ((Area[Count+1][InnerCount] == '#') || (Area[Count+1][InnerCount] == '~'))
          {
            LeftBounded = RightBounded = false;
            SideCount=1;
            while ((Area[Count][InnerCount-SideCount] != '#') && ((Area[Count+1][InnerCount-SideCount] == '#') || (Area[Count+1][InnerCount-SideCount] == '~')))
              {
            if (Area[Count][InnerCount-SideCount]=='.')
              {
                Area[Count][InnerCount-SideCount]='|';
                Changed=true;
              }
            SideCount++;
              }
            if (Area[Count][InnerCount-SideCount] == '#')
              {
            LeftBounded = true;
            LeftBound = -SideCount;
              }
            else
              {
            if (Area[Count][InnerCount-SideCount]=='.')
              {
                Area[Count][InnerCount-SideCount]='|';
                Changed=true;
              }
              }
            SideCount=1;
            while ((Area[Count][InnerCount+SideCount] != '#') && ((Area[Count+1][InnerCount+SideCount] == '#') || (Area[Count+1][InnerCount+SideCount] == '~')))
              {
            if (Area[Count][InnerCount+SideCount]=='.')
              {
                Area[Count][InnerCount+SideCount]='|';
                Changed=true;
              }
            SideCount++;
              }
            if (Area[Count][InnerCount+SideCount] == '#')
              {
            RightBounded = true;
            RightBound = SideCount;
              }
            else
              {
            if (Area[Count][InnerCount+SideCount]=='.')
              {
                Area[Count][InnerCount+SideCount]='|';
                Changed=true;
              }
              }
            if (LeftBounded && RightBounded)
              {
            for (SideCount = LeftBound+1;SideCount < RightBound; SideCount++)
              {
                if (Area[Count][InnerCount+SideCount]!='~')
                  {
                Area[Count][InnerCount+SideCount]='~';
                Changed=true;
                  }
              }
              }
          }
          }
      }
    }
  printf("Final:\n");
  for (Count=0;Count<=YMax;Count++)
    {
      for (InnerCount=(XMin-2);InnerCount<=(XMax+2);InnerCount++)
    printf("%c", Area[Count][InnerCount]);
      printf("\n");
    }
  TotalArea=0;
  for (Count=YMin;Count<=YMax;Count++)
    for (InnerCount=(XMin-2);InnerCount<=(XMax+2);InnerCount++)
      if ((Area[Count][InnerCount] == '|') || (Area[Count][InnerCount] == '~'))
    {
      TotalArea++;
    }
  printf("Total:, %d", TotalArea);
}

#endif








