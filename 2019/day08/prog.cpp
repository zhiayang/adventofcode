// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"

int main()
{
	auto input = util::readFile("input.txt");

	constexpr int width  = 25;
	constexpr int height = 6;

	std::vector<util::grid<char, width, height>> layers;

	{
		int cx = 0;
		int cy = 0;

		layers.push_back({ });
		for(char c : input)
		{
			layers.back()(cx, cy) = c;
			cx++;

			if(cx == width)
				cx = 0, cy++;

			if(cy == height)
			{
				cx = 0, cy = 0;
				layers.push_back({ });
			}
		}


		int min_zeroes = INT_MAX;
		size_t min_zero_layer = 0;

		for(size_t i = 0; i < layers.size(); i++)
		{
			int num_zeroes = layers[i].count('0');
			if(num_zeroes < min_zeroes)
			{
				min_zeroes = num_zeroes;
				min_zero_layer = i;
			}
		}

		int num_1s = layers[min_zero_layer].count('1');
		int num_2s = layers[min_zero_layer].count('2');

		zpr::println("part 1: %d", num_1s * num_2s);
	}



	{
		std::vector<char> final;
		for(size_t i = 0; i < width * height; i++)
		{
			char x = 0;
			for(size_t l = 0; l < layers.size(); l++)
			{
				if(layers[l][i] == '0')
				{
					x = ' ';
					break;
				}
				else if(layers[l][i] == '1')
				{
					x = '#';
					break;
				}
			}

			final.push_back(x);
		}


		zpr::println("part 2:");
		for(int y = 0; y < height; y++)
		{
			for(int x = 0; x < width; x++)
				zpr::print("%c", final[y * width + x]);

			zpr::println("");
		}
	}
}











