// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

static int64_t eval(str_view& str, int (*get_prec)(char));
static int64_t eval1(str_view& str, int (*get_prec)(char))
{
	if(str[0] == '(')
	{
		auto s = str.drop(1);
		auto ret = eval(s, get_prec);

		str = s.drop(1);
		return ret;
	}
	else
	{
		size_t n = 0;
		while(n < str.size() && '0' <= str[n] && str[n] <= '9')
			n++;

		int64_t ret = to_int(str.take(n));
		str.remove_prefix(1);
		return ret;
	}
}

static int64_t evalRhs(str_view& str, int64_t lhs, int prio, int (*get_prec)(char))
{
	if(str.empty() || prio == -1)
		return lhs;

	while(!str.empty())
	{
		auto oper = str[0];
		auto prec = get_prec(oper);

		if(prec < prio)
			return lhs;

		str.remove_prefix(1);

		auto rhs = eval1(str, get_prec);
		auto next = str.empty() ? -1 : get_prec(str[0]);

		if(next > prec)
			rhs = evalRhs(str, rhs, prec + 1, get_prec);

		lhs = (oper == '+')
			? lhs + rhs
			: lhs * rhs;
	}

	return lhs;
}

static int64_t eval(str_view& str, int (*get_prec)(char))
{
	if(str.empty())
		return 0;

	int64_t lhs = eval1(str, get_prec);
	return evalRhs(str, lhs, 0, get_prec);
}

int main()
{
	auto lines = map(readFileLines("input.txt"), [](auto& x) { return replace(x.str(), " ", ""); });

	int64_t sum1 = 0;
	int64_t sum2 = 0;
	for(auto& line : lines)
	{
		auto sv1 = str_view(line);
		auto sv2 = str_view(line);

		sum1 += eval(sv1, [](char op) { return op == '+' || op == '*' ? 10 : -1; });
		sum2 += eval(sv2, [](char op) { return op == '+' ? 10 : op == '*' ? 5 : -1; });
	}

	zpr::println("part 1 = {}", sum1);
	zpr::println("part 2 = {}", sum2);
}
