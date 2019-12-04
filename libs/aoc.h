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

			size_t n = fsize - 1;
			while(n > 0 && s[n] == '\n')
				n--;

			input = std::string(s, n);
			delete[] s;
		}

		return input;
	}

	static inline std::string_view readFileRaw(const std::string& path)
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

			size_t n = fsize - 1;
			while(n > 0 && s[n] == '\n')
				n--;

			return std::string_view(s, n + 1);
		}
	}

	static inline std::vector<std::string_view> splitString(std::string_view view, char delim = '\n')
	{
		std::vector<std::string_view> ret;

		while(true)
		{
			size_t ln = view.find(delim);

			if(ln != std::string_view::npos)
			{
				ret.emplace_back(view.data(), ln);
				view.remove_prefix(ln + 1);
			}
			else
			{
				break;
			}
		}

		// account for the case when there's no trailing newline, and we still have some stuff stuck in the view.
		if(!view.empty())
			ret.emplace_back(view.data(), view.length());

		return ret;
	}

	static inline std::vector<std::string_view> readFileLines(const std::string& path, char delim = '\n')
	{
		return splitString(readFileRaw(path), delim);
	}
}

struct v2
{
	v2() : x(0), y(0) { }
	v2(int x, int y) : x(x), y(y) { }

	int x;
	int y;
};

inline bool operator == (const v2& a, const v2& b) { return a.x == b.x && a.y == b.y; }
inline bool operator < (const v2& a, const v2& b) { return a.x < b.x ? true : (b.x < a.x ? false : (a.y < b.y ? true : false)); }

inline bool operator > (const v2& a, const v2& b) { return b < a; }
inline bool operator != (const v2& a, const v2& b) { return !(a == b); }
inline bool operator <= (const v2& a, const v2& b) { return !(b < a); }
inline bool operator >= (const v2& a, const v2& b) { return !(a < b); }

inline v2 operator + (const v2& a, const v2& b) { return v2(a.x + b.x, a.y + b.y); }
inline v2 operator - (const v2& a, const v2& b) { return v2(a.x - b.x, a.y - b.y); }

inline v2& operator += (v2& a, const v2& b) { a.x += b.x; a.y += b.y; return a; }
inline v2& operator -= (v2& a, const v2& b) { a.x -= b.x; a.y -= b.y; return a; }

struct v3
{
	v3() : x(0), y(0), z(0) { }
	v3(int x, int y, int z) : x(x), y(y), z(z) { }

	int x;
	int y;
	int z;
};

inline bool operator == (const v3& a, const v3& b) { return a.x == b.x && a.y == b.y && a.z == b.z; }
inline bool operator < (const v3& a, const v3& b) { return a.x < b.x ? true : (b.x < a.x ? false : (a.y < b.y ? true : (b.y < a.y ? false : (a.z < b.z ? true : false)))); }

inline bool operator > (const v3& a, const v3& b) { return b < a; }
inline bool operator != (const v3& a, const v3& b) { return !(a == b); }
inline bool operator <= (const v3& a, const v3& b) { return !(b < a); }
inline bool operator >= (const v3& a, const v3& b) { return !(a < b); }

inline v3 operator + (const v3& a, const v3& b) { return v3(a.x + b.x, a.y + b.y, a.z + b.z); }
inline v3 operator - (const v3& a, const v3& b) { return v3(a.x - b.x, a.y - b.y, a.z - b.z); }

inline v3& operator += (v3& a, const v3& b) { a.x += b.x; a.y += b.y; a.z += b.z; return a; }
inline v3& operator -= (v3& a, const v3& b) { a.x -= b.x; a.y -= b.y; a.z -= b.z; return a; }



