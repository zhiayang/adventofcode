// aoc.h
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#pragma once

#include <set>
#include <queue>
#include <fstream>
#include <sstream>
#include "assert.h"

#include "zpr.h"
#include "utils.h"


namespace util
{
	static inline std::string readFile(const std::string& path)
	{
		FILE* f = fopen(path.c_str(), "r");
		if(!f) fprintf(stderr, "failed to open file '%s'\n", path.c_str()), abort();

		std::string input;
		{
			fseek(f, 0, SEEK_END);

			long fsize = ftell(f);
			fseek(f, 0, SEEK_SET);  //same as rewind(f);

			char* s = new char[fsize + 1];
			fread(s, fsize, 1, f);
			fclose(f);
			s[fsize] = 0;

			size_t n = fsize;
			while(n > 0 && s[--n] == '\n')
				;

			input = std::string(s, n + 1);
			delete[] s;
		}

		return input;
	}

	static inline std::string_view readFileRaw(const std::string& path)
	{
		FILE* f = fopen(path.c_str(), "r");
		if(!f) fprintf(stderr, "failed to open file '%s'\n", path.c_str()), abort();

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


	static inline std::string_view trim(std::string_view s)
	{
		auto ltrim = [](std::string_view& s) -> std::string_view& {
			auto i = s.find_first_not_of(" \t\n\r\f\v");
			if(i != -1) s.remove_prefix(i);

			return s;
		};

		auto rtrim = [](std::string_view& s) -> std::string_view& {
			auto i = s.find_last_not_of(" \t\n\r\f\v");
			if(i != -1) s = s.substr(0, i + 1);

			return s;
		};

		return ltrim(rtrim(s));
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

	template <typename T, typename AdjacentFn, typename CostFn>
	static inline std::vector<T> astarSearch(const T& source, const T& dest, AdjacentFn adjFn, CostFn costFn)
	{
		struct wrapper_t
		{
			wrapper_t(const T& v, int d) : path({ v }), d(d) { }
			wrapper_t(const wrapper_t& w, const T& v, int d)
			{
				this->path = w.path;
				this->path.push_back(v);

				this->d = d;
			}

			std::vector<T> path;
			int d = 0;

			bool operator > (const wrapper_t& x) const { return this->d > x.d; }
		};

		auto queue = std::priority_queue<wrapper_t, std::vector<wrapper_t>, std::greater<wrapper_t>>();
		queue.emplace(source, 0 + costFn(source, dest));

		std::set<T> visited;
		visited.insert(source);

		while(!queue.empty())
		{
			auto path = queue.top();
			queue.pop();

			const auto& x = path.path.back();
			auto adjs = adjFn(x);

			for(const auto& n : adjs)
			{
				if(visited.find(n) == visited.end())
				{
					visited.insert(n);

					auto newpath = wrapper_t(path, n, static_cast<int>(path.path.size()) + costFn(n, dest));
					if(n == dest)
						return newpath.path;

					queue.push(newpath);
				}
			}
		}

		return { };
	}

	template <typename T>
	class queue : public std::deque<T>
	{
		using std::deque<T>::deque;

	public:
		T pop()
		{
			auto x = this->front();
			this->pop_front();
			return x;
		}

		void push(const T& x)
		{
			this->push_back(x);
		}

		void push_all(const std::vector<T>& xs)
		{
			for(const auto& x : xs)
				this->push(x);
		}
	};


	template <typename T, size_t W, size_t H>
	class grid
	{
		static_assert(W > 0 && H > 0, "invalid dimensions");

	public:
		grid()
		{
			this->array = new T[W * H];
		}

		~grid()
		{
			delete[] this->array;
		}

		grid(const grid& g)
		{
			this->array = new T[W * H];
			for(size_t i = 0; i < W * H; i++)
				this->array[i] = g.array[i];
		}

		grid& operator = (const grid& g)
		{
			if(this != &g)
			{
				delete[] this->array;

				this->array = new T[W * H];
				for(size_t i = 0; i < W * H; i++)
					this->array[i] = g.array[i];
			}

			return *this;
		}

		const T* raw() const
		{
			return this->array;
		}

		T* raw()
		{
			return this->array;
		}

		T operator [] (size_t i) const
		{
			return this->array[i];
		}

		T& operator [] (size_t i)
		{
			return this->array[i];
		}

		size_t width() const
		{
			return W;
		}

		size_t height() const
		{
			return H;
		}

		T& operator() (size_t x, size_t y)
		{
			if(x >= W || y >= H)
				assert(!"out of bounds");
			return this->array[x + y * W];
		}

		T operator() (size_t x, size_t y) const
		{
			if(x >= W || y >= H)
				assert(!"out of bounds");
			return this->array[x + y * W];
		}

		// helpers
		size_t count(const T& x) const
		{
			size_t cnt = 0;
			for(size_t i = 0; i < W * H; i++)
				if(this->array[i] == x) cnt++;

			return cnt;
		}

		template <typename U, typename Fn>
		grid<U, W, H> map(Fn f)
		{
			auto ret = grid<U, W, H>();
			for(size_t x = 0; x < W; x++)
			{
				for(size_t y = 0; y < H; y++)
				{
					ret(x, y) = f(x, y, this->operator()(x, y));
				}
			}

			return ret;
		}



	private:
		T* array;
	};
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

namespace zpr
{
	template <>
	struct print_formatter<v2>
	{
		std::string print(const v2& v, const format_args& args)
		{
			return zpr::sprint("(%d, %d)", v.x, v.y);
		}
	};

	template <>
	struct print_formatter<v3>
	{
		std::string print(const v3& v, const format_args& args)
		{
			return zpr::sprint("(%d, %d, %d)", v.x, v.y, v.z);
		}
	};
}

namespace util
{
	double distance(const v2& a, const v2& b)
	{
		return std::sqrt( (b.x - a.x) * (b.x - a.x)
						+ (b.y - a.y) * (b.y - a.y));
	}

	double manhattan_dist(const v2& a, const v2& b)
	{
		return std::abs(b.x - a.x) + std::abs(b.y - a.y);
	}
}











