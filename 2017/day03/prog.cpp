// prog.cpp
// Copyright (c) 2014 - 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include <stdio.h>

#include <map>
#include <string>
#include <algorithm>

int main()
{
	int input = 361527;


	#if 0
	{
		// get the closest square smaller than it.
		// we skip 2 because only odd squares end up at the bottom right corner.
		int x = 1;
		for(; x * x < input; x += 2)
			;

		// move one square-size backwards
		// x -= 2;
		int corner = x * x;

		// x is the size of the square.
		// y is the distance from a corner the centre of the nearest edge.
		int y = (x - 1) / 2;

		// a, b, c, and d are the numbers at the centres of the 4 edges
		int a = corner - y;
		int b = a - (2 * y);
		int c = b - (2 * y);
		int d = c - (2 * y);

		printf("x = %d\n", x);
		printf("(%d, %d, %d, %d)\n", a, b, c, d);

		// find the distance from the number itself to the nearest centre edge.
		int d1 = std::min({ std::abs(input - a), std::abs(input - b), std::abs(input - c), std::abs(input - d) });
		printf("d1 = %d\n", d1);

		// then the remaining is just from the edge to the centre, which is 'y'
		printf("dist = %d\n", d1 + y);
	}
	#else
	{
		/*
			is there a pattern?
			1	1	2	4	5	10	11	23	25	26	54	57	59	122	133	142	147	304	330	351	362	747	806
			0	0	1	2	1	5	1	12	2	1	28	3	2	63	11	9	5	157	26	21	11	365	59

		*/

		std::map<std::pair<int, int>, int> map;
		auto get = [&map](int x, int y) -> int {
			if(x == 0 && y == 0)
				return 1;

			return map[{ x, y }];
		};

		auto set = [&map, &get](int x, int y) -> int {
			// get the neighbours
			int a = get(x - 1, y + 1);
			int b = get(x, y + 1);
			int c = get(x + 1, y + 1);
			int d = get(x + 1, y);
			int e = get(x + 1, y - 1);
			int f = get(x, y - 1);
			int g = get(x - 1, y - 1);
			int h = get(x - 1, y);

			/*
				a b c
				h X d
				g f e
			*/

			auto n = a + b + c + d + e + f + g + h;
			map[{ x, y }] = n;

			return n;
		};

		// do the spiral

		int maxx = 100;
		int maxy = 100;

		int x = 0;
		int y = 0;
		int dx = 0;
		int dy = -1;

		int i = 1;

		struct SpiralMachine
		{
			int x = 0;
			int y = 0;

			// sorry about the lack of creative names.
			int n = 1;
			int direction = 0;

			/*
				how this works:
				basically given this grid,


				17	16	15	14	13
				18	5	4	3	12
				19	6	1	2	11
				20	7	8	9	10
				21	22	23	24	25

				we can split it into pieces
				7	7	7	7	6
				8	3	3	2	6	.
				8	4		1	6	.
				8	4	5	5	5	.
				8	9	9	9	9	9

				the numbers progess in 4 directions, that's 'direction' above.
				0 = right, 1 = up, 2 = left, 3 = down

				'n' is the size of each segment, as it were.
				first we move right, so x == n. we increment 'direction', so now we're moving up.

				now we're moving up, so we increment y: y == 1 == n. again we increment 'direction', so now
				we move left.

				we're moving left, so we decrement x: x = 0.
				again, x = -1.

				now, -x == n == 1. we move down.
				y = 0.
				y = -1.
				-y == n -- finally we increment n, so n == 2.

				moving right:
				x = 0
				x = 1
				x = 2
				x == n -- move up.

				y = 0
				y = 1
				y = 2
				y == n -- move left

				x = 1
				x = 0
				x = -1
				x = -2
				-x == n -- move down

				y = 1
				y = 0
				y = -1
				y = -2
				-y == n -- move right, n++

				x = -1
				x = 0
				x = 1
				x = 2
				x = 3

				and etc. etc.

				so we only increment 'n' when we reach -y == n.
			*/

			std::pair<int, int> next()
			{
				switch(this->direction)
				{
					case 0:
						// right
						this->x += 1;
						if(this->x == this->n)
							this->direction++;

						break;

					case 1:
						// up
						this->y += 1;
						if(this->y == this->n)
							this->direction++;

						break;

					case 2:
						//left
						this->x -= 1;
						if(this->x == -1 * this->n)
							this->direction++;

						break;

					case 3:
						// down
						this->y -= 1;
						if(this->y == -1 * this->n)
							this->direction++, this->n++;

						break;

					default:
						abort();
				}

				this->direction %= 4;
				return { this->x, this->y };
			}
		};


		SpiralMachine sm;
		while(true)
		{
			int x = 0; int y = 0;
			std::tie(x, y) = sm.next();

			int n = set(x, y);
			if(n > input)
			{
				printf("n = %d\n", n);
				break;
			}
		}
	}
	#endif

	return 0;
}








