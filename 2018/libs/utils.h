// utils.h
// Copyright (c) 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#pragma once
#include <string>
#include <vector>
#include <unordered_map>

template <typename T>
std::vector<T> operator + (const std::vector<T>& vec, const T& elm)
{
	auto copy = vec;

	copy.push_back(elm);
	return copy;
}

template <typename T>
std::vector<T> operator + (const T& elm, const std::vector<T>& vec)
{
	auto copy = vec;

	copy.insert(copy.begin(), elm);
	return copy;
}

template <typename T>
std::vector<T> operator + (const std::vector<T>& a, const std::vector<T>& b)
{
	auto ret = a;

	ret.insert(ret.begin(), b.begin(), b.end());
	return ret;
}

struct v2
{
	v2() : x(0), y(0) { }
	v2(int x, int y) : x(x), y(y) { }

	int x;
	int y;
};

bool operator == (const v2& a, const v2& b) { return a.x == b.x && a.y == b.y; }
bool operator < (const v2& a, const v2& b) { return a.x < b.x ? true : (b.x < a.x ? false : (a.y < b.y ? true : false)); }

bool operator > (const v2& a, const v2& b) { return b < a; }
bool operator != (const v2& a, const v2& b) { return !(a == b); }
bool operator <= (const v2& a, const v2& b) { return !(b < a); }
bool operator >= (const v2& a, const v2& b) { return !(a < b); }


namespace util
{
	template <typename T>
	bool match(const T& first)
	{
		return true;
	}

	template <typename T, typename U>
	bool match(const T& first, const U& second)
	{
		return (first == second);
	}

	template <typename T, typename U, typename... Args>
	bool match(const T& first, const U& second, const Args&... comps)
	{
		return (first == second) || match(first, comps...);
	}


	template <typename T, class UnaryOp, typename K = typename std::result_of<UnaryOp(T)>::type>
	std::vector<K> map(const std::vector<T>& input, UnaryOp fn)
	{
		std::vector<K> ret;
		for(auto i : input)
			ret.push_back(fn(i));

		return ret;
	}



	template <typename T, class Predicate>
	std::vector<T> sort(const std::vector<T>& input, Predicate fn)
	{
		std::vector<T> ret = input;
		std::sort(ret.begin(), ret.end(), fn);

		return ret;
	}


	template <typename T, class UnaryOp, typename K = typename std::result_of<UnaryOp(T, size_t)>::type>
	std::vector<K> mapidx(const std::vector<T>& input, UnaryOp fn)
	{
		std::vector<K> ret;
		for(size_t i = 0; i < input.size(); i++)
			ret.push_back(fn(input[i], i));

		return ret;
	}



	template <typename T, class UnaryOp, class Predicate, typename K = typename std::result_of<UnaryOp(T)>::type>
	std::vector<K> filterMap(const std::vector<T>& input, Predicate cond, UnaryOp fn)
	{
		std::vector<K> ret;
		for(auto i : input)
		{
			if(cond(i))
				ret.push_back(fn(i));
		}

		return ret;
	}

	template <typename T, class UnaryOp, class Predicate, typename K = typename std::result_of<UnaryOp(T)>::type>
	std::vector<K> mapFilter(const std::vector<T>& input, UnaryOp fn, Predicate cond)
	{
		std::vector<K> ret;
		for(auto i : input)
		{
			auto k = fn(i);
			if(cond(k)) ret.push_back(k);
		}

		return ret;
	}

	template <typename T, class Predicate>
	std::vector<T> filter(const std::vector<T>& input, Predicate cond)
	{
		std::vector<T> ret;
		for(const auto& i : input)
			if(cond(i))
				ret.push_back(i);

		return ret;
	}

	template <typename T, class Predicate>
	std::vector<T> filterUntil(const std::vector<T>& input, Predicate cond)
	{
		std::vector<T> ret;
		for(const auto& i : input)
		{
			if(cond(i)) ret.push_back(i);
			else        break;
		}

		return ret;
	}

	template <typename T, class Predicate>
	size_t indexOf(const std::vector<T>& input, Predicate cond)
	{
		for(size_t i = 0; i < input.size(); i++)
			if(cond(input[i])) return i;

		return -1;
	}

	template <typename T>
	std::vector<T> take(const std::vector<T>& v, size_t num)
	{
		return std::vector<T>(v.begin(), v.begin() + num);
	}

	inline std::string join(const std::vector<std::string>& list, const std::string& sep)
	{
		if(list.empty())            return "";
		else if(list.size() == 1)   return list[0];

		std::string ret;
		for(size_t i = 0; i < list.size() - 1; i++)
			ret += list[i] + sep;

		return ret + list.back();
	}





	inline std::string serialiseScope(const std::vector<std::string>& scope)
	{
		if(scope.empty()) return "";

		std::string ret = scope[0];
		for(size_t i = 1; i < scope.size(); i++)
			ret += "::" + scope[i];

		return ret;
	}

	inline std::string plural(const std::string& thing, size_t count)
	{
		return thing + (count == 1 ? "" : "s");
	}

	template <typename T, class UnaryOp>
	std::string listToString(const std::vector<T>& list, UnaryOp fn)
	{
		std::string ret;
		for(size_t i = 0; i < list.size(); i++)
		{
			ret += fn(list[i]);
			if(i != list.size() - 1)
				ret += ", ";
		}

		return "[ " + ret + " ]";
	}

	template <typename K, typename V>
	std::vector<std::pair<K, V>> pairs(const std::unordered_map<K, V>& map)
	{
		auto ret = std::vector<std::pair<K, V>>(map.begin(), map.end());
		return ret;
	}


	struct path_t
	{
		path_t(int w, const std::vector<v2>& ns) : weight(w), nodes(ns) { }

		int weight;
		std::vector<v2> nodes;
	};

	int linedist(const v2& a, const v2& b)
	{
		return (int) std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
	}

	path_t pathfind(const std::map<v2, std::set<v2>>& edges, const v2& from, const v2& dest)
	{
		std::set<v2> seen;
		std::deque<path_t> queue = { path_t(linedist(from, dest), { from }) };

		while(queue.size() > 0)
		{
			auto [ w, ns ] = queue.front();
			queue.pop_front();

			auto neighs = edges.at(ns.back());
			for(const v2& n : neighs)
			{
				if(seen.find(n) != seen.end())
					continue;

				seen.insert(n);

				auto newpath = path_t(ns.size() + linedist(n, dest), ns + n);

				if(n == dest)
					return newpath;

				else
					queue.push_back(newpath);
			}

			std::sort(queue.begin(), queue.end(), [](const path_t& p1, const path_t& p2) -> bool {
				return p1.weight < p2.weight;
			});
		}

		return path_t(0, { });
	}
}



#define COLOUR_RESET			"\033[0m"
#define COLOUR_BLACK			"\033[30m"			// Black
#define COLOUR_RED				"\033[31m"			// Red
#define COLOUR_GREEN			"\033[32m"			// Green
#define COLOUR_YELLOW			"\033[33m"			// Yellow
#define COLOUR_BLUE				"\033[34m"			// Blue
#define COLOUR_MAGENTA			"\033[35m"			// Magenta
#define COLOUR_CYAN				"\033[36m"			// Cyan
#define COLOUR_WHITE			"\033[37m"			// White
#define COLOUR_BLACK_BOLD		"\033[1m"			// Bold Black
#define COLOUR_RED_BOLD			"\033[1m\033[31m"	// Bold Red
#define COLOUR_GREEN_BOLD		"\033[1m\033[32m"	// Bold Green
#define COLOUR_YELLOW_BOLD		"\033[1m\033[33m"	// Bold Yellow
#define COLOUR_BLUE_BOLD		"\033[1m\033[34m"	// Bold Blue
#define COLOUR_MAGENTA_BOLD		"\033[1m\033[35m"	// Bold Magenta
#define COLOUR_CYAN_BOLD		"\033[1m\033[36m"	// Bold Cyan
#define COLOUR_WHITE_BOLD		"\033[1m\033[37m"	// Bold White
#define COLOUR_GREY_BOLD		"\033[30;1m"		// Bold Grey

