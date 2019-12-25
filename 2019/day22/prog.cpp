// prog.cpp
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc.h"

using int_t = __int128_t;

constexpr int_t NUM_CARDS_P1       = 10007;
constexpr int_t NUM_CARDS_P2       = 119315717514047;
constexpr int_t NUM_SHUFFLES       = 101741582076661;


// because c++ doesn't do 'mathematical' modulo, ie. -2 % 33 is supposed to be 31, not -2
static int_t mod(int_t a, int_t b)
{
	return (b + (a % b)) % b;
}

static int_t ext_gcd(int_t a, int_t b, int_t* x, int_t* y)
{
	if(b == 0)
	{
		*x = 1;
		*y = 0;
		return a;
	}

	int_t x1 = 0;
	int_t y1 = 0;

	auto gcd = ext_gcd(b, a % b, &x1, &y1);

	*x = y1;
	*y = x1 - (a / b) * y1;

	return gcd;
}

int main()
{
	// fuck you, topaz. get coal in your fucking socks
	// https://www.reddit.com/r/adventofcode/comments/ee0rqi/2019_day_22_solutions/fbnkaju/

	struct Deck
	{
		int_t first;
		int_t incrm;

		int_t size;
	};


	auto modular_inverse = [](int_t a, int_t n) -> int_t {
		int_t x = 0; int_t y = 0;

		if(a < n) std::swap(a, n);
		ext_gcd(a, n, &x, &y);

		// y is the inverse.
		return y;
	};


	auto reverse = [](Deck deck) -> Deck {
		// since the cards go in reverse now, the increment is negated.
		deck.incrm *= -1;

		// the first card is now the last card, and vice-versa. so, we can
		// go "backwards" and due to % we'll get the last card. (note: incrm is already
		// negated here)
		deck.first += deck.incrm;

		deck.first = mod(deck.first, deck.size);
		deck.incrm = mod(deck.incrm, deck.size);
		return deck;
	};

	auto cut = [](Deck deck, int_t n) -> Deck {
		// the nth card becomes the first card.
		deck.first += deck.incrm * n;
		deck.first = mod(deck.first, deck.size);

		return deck;
	};

	auto deal = [&modular_inverse](Deck deck, int_t n) -> Deck {
		// the first card stays the same. for subsequent cards, the 0-card goes to 0,
		// 1-card goes to N, 2-card to 2N, ... k-card to k*N. since we want the
		// 1-card, we want some K for K*N = 1. ie. we want the multiplicative inverse (mod S) of N
		// (where S is the card size).

		// something something bezout identity
		// there exist x, y st. ax + by = 1  <=>  gcd(a, b) = 1
		// int_t x = 0; int_t y = 0;
		// ext_gcd(n, deck.size, &x, &y);

		// y is the inverse.
		auto invN = modular_inverse(n, deck.size);

		// the 0-card is deck.first
		// the 1-card is deck.first + (deck.incrm * invN)
		// so the difference (ie. the new increment) is deck.incrm * invN
		deck.incrm = mod(deck.incrm * invN, deck.size);

		// 0 1 2 3 4 5 6   <- deal 4
		// 0 2 4 6 1 3 5   <- deal 2
		// 0 1 2 3 4 5 6

		return deck;
	};

	// get the nth card in the deck.
	auto get = [](Deck deck, int_t n) -> int_t {
		return mod(deck.first + mod(deck.incrm * n, deck.size), deck.size);
	};




	auto shuffle = [&cut, &deal, &reverse](const std::vector<std::string_view>& lines, Deck deck) -> Deck {

		for(const auto& _move : lines)
		{
			auto m = std::string(_move);

			if(m.find("cut") == 0)
			{
				int n = 0;
				sscanf(m.c_str(), "cut %d", &n);

				deck = cut(deck, n);
			}
			else if(m.find("deal with") == 0)
			{
				int n = 0;
				sscanf(m.c_str(), "deal with increment %d", &n);

				deck = deal(deck, n);
			}
			else if(m.find("deal into") == 0)
			{
				deck = reverse(deck);
			}
		}

		return deck;
	};



	auto lines = util::readFileLines("input.txt");
	{
		// setup: first card is 0, next card increases by 1.
		Deck deck;

		deck.first = 0;
		deck.incrm = 1;
		deck.size = NUM_CARDS_P1;

		deck = shuffle(lines, deck);

		// for part 1, since it wants the position of a *certain card*
		// (versus the card at a *certain position*), we can just do a search
		// for card 2019, from n=0 to n=10006.
		for(size_t i = 0; i < deck.size; i++)
		{
			if(get(deck, i) == 2019)
			{
				zpr::println("part 1: %d", i);
				break;
			}
		}
	}


	{
		Deck deck;
		deck.first = 0;
		deck.incrm = 1;
		deck.size = NUM_CARDS_P2;

		// just do it again, cos i'm lazy.
		deck = shuffle(lines, deck);
		auto offs = deck.first;
		auto incr = deck.incrm;

		// the increment originally was 1; the new increment is whatever it is now, after 1 shuffle.
		// after two shuffles, the increment is squared, after 3 it's cubed, you get the idea. (modulo N, of course)
		// to get the increment for the final deck after that dumb number of shuffles,
		// we need to calculate incr^NUM_SHUFFLES (mod NUM_CARDS_P2).

		std::function<int_t (int_t a, int_t x, int_t N)> fast_exp;
		fast_exp = [&fast_exp](int_t a, int_t x, int_t N) -> int_t {

			// this is basically exponentiation by squaring. 2^16 = 4^8 = 16^4 = 256^2
			if(x == 1) return a % N;

			if(x % 2 == 0)
			{
				return fast_exp((a * a) % N, x / 2, N) % N;
			}
			else
			{
				return a * (fast_exp((a * a) % N, (x - 1) / 2, N) % N) % N;
			}
		};

		auto increment = fast_exp(incr, NUM_SHUFFLES, deck.size);


		// this is the pattern for offset, which isn't so simple:
		// note: offset_incr is the offset after 1 shuffle.

		// n=0, offset = 0
		// n=1, offset = offset_incr
		// n=2, offset = offset_incr + (incr * offset_incr)
		// n=3, offset = offset_incr + (incr * offset_incr) + ((incr^2) * offset_incr)
		// ...

		// factorising,
		// offset = offset_incr * (1 + incr + incr^2 + incr^3 + ... + incr^(n-1))
		// ie to say it's a geometric progression. Sn = (a * (1 - r^n)) / (1-r)
		// (the first N terms of a GP go up to power N-1, so that's good for us)

		// so finally:
		auto offset = mod(mod(offs * (1 - increment), deck.size) * modular_inverse(1 - incr, deck.size), deck.size);

		// make a deck:
		deck.first = offset;
		deck.incrm = increment;

		zpr::println("part 2: %d", (int64_t) get(deck, 2020));
	}
}




