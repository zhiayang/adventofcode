// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 14


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <map>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>


struct Reindeer
{
	Reindeer(std::string n, size_t s, size_t f, size_t r) : name(n), speed(s), flight(f), rest(r) { }

	std::string name = "";
	size_t speed = 0;

	size_t flight = 0;
	size_t rest = 0;

	size_t dist = 0;
	size_t flightTime = 0;
	size_t restCounter = 0;


	size_t points = 0;


	// 0 = flying
	// 1 = resting
	int state = 0;
};

int main()
{
	size_t duration = 2503;
	std::vector<Reindeer*> reindeers;

	{
		reindeers.push_back(new Reindeer("Dancer", 27, 5, 132));
		reindeers.push_back(new Reindeer("Cupid", 22, 2, 41));
		reindeers.push_back(new Reindeer("Rudolph", 11, 5, 48));
		reindeers.push_back(new Reindeer("Donner", 28, 5, 134));
		reindeers.push_back(new Reindeer("Dasher", 4, 16, 55));
		reindeers.push_back(new Reindeer("Blitzen", 14, 3, 38));
		reindeers.push_back(new Reindeer("Prancer", 3, 21, 40));
		reindeers.push_back(new Reindeer("Comet", 18, 6, 103));
		reindeers.push_back(new Reindeer("Vixen", 18, 5, 84));
	}


	for(size_t i = 0; i < duration; i++)
	{
		Reindeer* best = reindeers.front();
		for(auto rn : reindeers)
		{
			if(rn->state == 0)
			{
				rn->flightTime++;
				rn->dist += rn->speed;

				if(rn->flightTime == rn->flight)
				{
					rn->state = 1;
					rn->flightTime = 0;
				}
			}
			else
			{
				rn->restCounter++;
				if(rn->restCounter == rn->rest)
				{
					rn->state = 0;
					rn->restCounter = 0;
				}
			}
		}


		std::vector<Reindeer*> bests;
		for(auto& rn : reindeers)
		{
			if(bests.size() == 0 || rn->dist >= bests[0]->dist)
			{
				if(bests.size() > 0 && rn->dist > bests[0]->dist)
					bests.clear();

				bests.push_back(rn);
			}
		}


		for(auto rn : bests)
		{
			rn->points++;
		}
	}






	Reindeer* bestD = reindeers.front();
	{
		for(auto rn : reindeers)
		{
			if(rn->dist > bestD->dist)
				bestD = rn;
		}
	}

	Reindeer* bestP = reindeers.front();
	{
		for(auto rn : reindeers)
		{
			if(rn->points > bestP->points)
				bestP = rn;
		}
	}


	printf("winner by distance = %s, travelled %zu km\n", bestD->name.c_str(), bestD->dist);
	printf("winner by points = %s, got %zu points\n", bestP->name.c_str(), bestP->points);
}





























