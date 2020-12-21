// prog2.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

int main()
{
	auto file = readFile("input.txt");

	std::string out =
	"#include <stdio.h>                                         \n"
	"#include <stdint.h>                                        \n"
	"struct num { int64_t x; };                                 \n"
	"num operator\"\"_n(unsigned long long x) {                 \n"
	"    return num { .x = (int64_t) x };                       \n"
	"}                                                          \n"
	"num operator* (num a, num b) { return num { a.x + b.x }; } \n"
	"num operator+ (num a, num b) { return num { a.x * b.x }; } \n"
	"int main() {                                               \n"
	"    num result = num { .x = 0 };                           \n";

	out.reserve(file.size() * 2);

	auto lines = split(file, "\n");
	for(auto& ln : lines)
	{
		out += "    result = result * (";
		for(char c : ln)
		{
			if(c == '+') out += '*';
			else if(c == '*') out += '+';
			else if('0' <= c && c <= '9')
				out += c, out += "_n";
			else if(c != ' ')
				out += c;
		}

		out += ");\n";
	}

	// drop the last plus
	out.pop_back();

	out += ";\n";
	out +=
	"    printf(\"answer = %lld\\n\", result.x);    \n"
	"}                                              \n";

	{
		auto file = fopen("uwu.cpp", "w");
		fwrite(out.c_str(), out.size(), 1, file);

		fclose(file);
	}

	system("cc -o uwu -std=c++17 uwu.cpp");
	system("./uwu");
}
