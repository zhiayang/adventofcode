// aoc.h
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#pragma once

#include <fstream>
#include <sstream>

#include "zpr.h"
#include "utils.h"

namespace util
{
	static inline std::string readFile(const std::string& path)
	{
		FILE* f = fopen(path.c_str(), "r");
		std::string input;
		{
			fseek(f, 0, SEEK_END);

			long fsize = ftell(f);
			fseek(f, 0, SEEK_SET);  //same as rewind(f);

			char* s = new char[fsize + 1];
			fread(s, fsize, 1, f);
			fclose(f);
			s[fsize] = 0;

			input = std::string(s);
			while(input.back() == '\n')
				input.pop_back();
		}

		return input;
	}

	static inline std::vector<std::string> readFileLines(const std::string& path, char delim = '\n', bool skipEmpty = true)
	{
		std::vector<std::string> lines;
		{
			auto input = std::ifstream(path, std::ios::in);
			for(std::string line; std::getline(input, line, delim); )
			{
				if(!skipEmpty || !line.empty())
					lines.push_back(line);
			}
		}

		return lines;
	}

	static inline std::vector<std::string> splitString(const std::string& s, char delim)
	{
		std::vector<std::string> lines;
		{
			auto input = std::stringstream(s);
			for(std::string line; std::getline(input, line, delim); )
				lines.push_back(line);
		}

		return lines;
	}
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

v2 operator + (const v2& a, const v2& b) { return v2(a.x + b.x, a.y + b.y); }
v2 operator - (const v2& a, const v2& b) { return v2(a.x - b.x, a.y - b.y); }

v2& operator += (v2& a, const v2& b) { a.x += b.x; a.y += b.y; return a; }
v2& operator -= (v2& a, const v2& b) { a.x -= b.x; a.y -= b.y; return a; }

struct v3
{
	v3() : x(0), y(0), z(0) { }
	v3(int x, int y, int z) : x(x), y(y), z(z) { }

	int x;
	int y;
	int z;
};

bool operator == (const v3& a, const v3& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
bool operator < (const v3& a, const v3& b) { return a.x < b.x ? true : (b.x < a.x ? false : (a.y < b.y ? true : (b.y < a.y ? false : (a.z < b.z ? true : false)))); }

bool operator > (const v3& a, const v3& b) { return b < a; }
bool operator != (const v3& a, const v3& b) { return !(a == b); }
bool operator <= (const v3& a, const v3& b) { return !(b < a); }
bool operator >= (const v3& a, const v3& b) { return !(a < b); }

v3 operator + (const v3& a, const v3& b) { return v3(a.x + b.x, a.y + b.y, a.z + b.z); }
v3 operator - (const v3& a, const v3& b) { return v3(a.x - b.x, a.y - b.y, a.z - b.z); }

v3& operator += (v3& a, const v3& b) { a.x += b.x; a.y += b.y; a.z += b.z; return a; }
v3& operator -= (v3& a, const v3& b) { a.x -= b.x; a.y -= b.y; a.z -= b.z; return a; }



