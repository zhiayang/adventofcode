// prog.cpp
// Copyright (c) 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.


#include "assert.h"

#include <map>
#include <set>
#include <stack>
#include <array>
#include <deque>
#include <string>
#include <vector>
#include <unordered_set>

#include "utils.h"
#include "tinyformat.h"

int main()
{
	std::vector<std::pair<char, char>> input = {
		{ 'C', 'P' },
		{ 'V', 'Q' },
		{ 'T', 'X' },
		{ 'B', 'U' },
		{ 'Z', 'O' },
		{ 'P', 'I' },
		{ 'D', 'G' },
		{ 'A', 'Y' },
		{ 'R', 'O' },
		{ 'J', 'E' },
		{ 'N', 'S' },
		{ 'X', 'H' },
		{ 'F', 'L' },
		{ 'S', 'I' },
		{ 'W', 'Q' },
		{ 'H', 'K' },
		{ 'K', 'Q' },
		{ 'E', 'L' },
		{ 'Q', 'O' },
		{ 'U', 'G' },
		{ 'L', 'O' },
		{ 'Y', 'G' },
		{ 'G', 'I' },
		{ 'M', 'I' },
		{ 'I', 'O' },
		{ 'A', 'N' },
		{ 'H', 'O' },
		{ 'T', 'O' },
		{ 'H', 'U' },
		{ 'A', 'I' },
		{ 'B', 'R' },
		{ 'V', 'T' },
		{ 'H', 'M' },
		{ 'C', 'A' },
		{ 'B', 'G' },
		{ 'L', 'Y' },
		{ 'T', 'J' },
		{ 'A', 'R' },
		{ 'X', 'L' },
		{ 'B', 'L' },
		{ 'A', 'F' },
		{ 'K', 'O' },
		{ 'W', 'M' },
		{ 'Z', 'N' },
		{ 'Z', 'S' },
		{ 'R', 'K' },
		{ 'Q', 'L' },
		{ 'G', 'O' },
		{ 'F', 'Y' },
		{ 'V', 'H' },
		{ 'E', 'I' },
		{ 'W', 'Y' },
		{ 'U', 'I' },
		{ 'F', 'K' },
		{ 'M', 'O' },
		{ 'Z', 'H' },
		{ 'X', 'S' },
		{ 'J', 'O' },
		{ 'B', 'I' },
		{ 'F', 'H' },
		{ 'D', 'U' },
		{ 'E', 'M' },
		{ 'Z', 'X' },
		{ 'P', 'L' },
		{ 'W', 'H' },
		{ 'C', 'D' },
		{ 'A', 'X' },
		{ 'Q', 'I' },
		{ 'R', 'Y' },
		{ 'B', 'A' },
		{ 'N', 'L' },
		{ 'H', 'G' },
		{ 'Y', 'M' },
		{ 'L', 'G' },
		{ 'G', 'M' },
		{ 'Z', 'R' },
		{ 'S', 'Q' },
		{ 'P', 'J' },
		{ 'V', 'J' },
		{ 'J', 'I' },
		{ 'J', 'X' },
		{ 'W', 'O' },
		{ 'B', 'F' },
		{ 'R', 'M' },
		{ 'V', 'S' },
		{ 'H', 'E' },
		{ 'E', 'U' },
		{ 'R', 'W' },
		{ 'X', 'Q' },
		{ 'N', 'G' },
		{ 'T', 'I' },
		{ 'L', 'M' },
		{ 'H', 'I' },
		{ 'U', 'M' },
		{ 'C', 'H' },
		{ 'P', 'H' },
		{ 'J', 'F' },
		{ 'A', 'O' },
		{ 'X', 'M' },
		{ 'H', 'L' },
		{ 'W', 'K' },

		// { 'C', 'A' },
		// { 'C', 'F' },
		// { 'A', 'B' },
		// { 'A', 'D' },
		// { 'B', 'E' },
		// { 'D', 'E' },
		// { 'F', 'E' },
	};

	struct letter_t
	{
		char letter;
		std::vector<letter_t*> dependsOn;

		bool satisfied = false;
	};

	#define NUM_LETTERS 26

	std::vector<letter_t> letters;
	for(int i = 0; i < NUM_LETTERS; i++)
	{
		auto l = letter_t();
		l.letter = 'A' + i;

		letters.push_back(l);
	}


	for(const auto& k : input)
	{
		// second depends on first.
		letters[k.second - 'A'].dependsOn.push_back(&letters[k.first - 'A']);
	}


	// see which ones are available!
	std::string final_order;
	{
		while(true)
		{
			bool changed = false;
			for(int i = 0; i < NUM_LETTERS; i++)
			{
				auto& let = letters[i];
				if(!let.satisfied)
				{
					// check all dependencies

					bool sats = true;
					for(auto dep : let.dependsOn)
						sats = sats && dep->satisfied;

					let.satisfied = sats;
					if(let.satisfied)
					{
						changed = true;
						final_order.push_back(let.letter);

						break;
					}
				}
			}

			if(!changed) break;
		}

		tfm::printfln("part 1: %s", final_order);
	}



	// reset
	for(auto& let : letters)
		let.satisfied = false;


	{
		struct worker_t
		{
			char working_on = 0;
			int time_left = 0;
		};

		#define NUM_WORKERS 5
		#define MIN_WORK_TIME 60

		std::deque<char> queue;
		std::deque<char> already_queued;
		std::array<worker_t, NUM_WORKERS> workers = { };

		std::string done;

		auto reduce_unemployment = [&workers, &queue, &letters, &done]() {
			for(size_t i = 0; i < NUM_WORKERS; i++)
			{
				if(workers[i].working_on == 0 && queue.size() > 0)
				{
					auto let = queue.front();
					queue.pop_front();

					workers[i].working_on = let;
					workers[i].time_left = 1 + MIN_WORK_TIME + (let - 'A');
				}
			}
		};

		auto tick_workers = [&workers, &queue, &letters, &done, &reduce_unemployment]() {
			reduce_unemployment();

			for(size_t i = 0; i < NUM_WORKERS; i++)
			{
				if(workers[i].time_left == 1 && workers[i].working_on != 0)
				{
					auto let = &letters[workers[i].working_on - 'A'];
					let->satisfied = true;
					workers[i].working_on = 0;

					done.push_back(let->letter);


					// see if there's more shit to do!
					if(queue.size() > 0)
					{
						auto let = queue.front();
						queue.pop_front();

						workers[i].working_on = let;
						workers[i].time_left = 1 + MIN_WORK_TIME + (let - 'A');
					}
				}
				else if(workers[i].working_on != 0)
				{
					workers[i].time_left--;
				}
			}
		};

		int tick = 0;
		tfm::printfln("tick    w1    w2    done        queue");
		while(true)
		{
			for(int i = 0; i < NUM_LETTERS; i++)
			{
				auto let = &letters[i];
				if(!let->satisfied)
				{
					// check all dependencies
					bool sats = true;
					for(auto dep : let->dependsOn)
						sats = sats && dep->satisfied;

					if(sats && std::find(already_queued.begin(), already_queued.end(), let->letter) == already_queued.end())
					{
						queue.push_back(let->letter);
						already_queued.push_back(let->letter);
						// tfm::printfln("add %c to queue!", let->letter);
						// reduce_unemployment();
						// break;
					}
				}
			}

			std::string q;
			for(auto l : queue)
				q += l;

			tfm::printfln("%4d    %c     %c     %-*s      %s", tick, workers[0].working_on, workers[1].working_on, NUM_LETTERS, done, q);

			tick_workers(); tick++;

			if(done.length() == NUM_LETTERS) break;
		}

		tfm::printfln("part 2: took %d ticks (%s)", tick, done);
	}
}





















