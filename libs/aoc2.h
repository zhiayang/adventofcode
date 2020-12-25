// aoc2.h
// Copyright (c) 2019, zhiayang
// Licensed under the Apache License Version 2.0.

#pragma once

#include <set>
#include <array>
#include <queue>
#include <chrono>
#include <fstream>
#include <sstream>
#include <optional>
#include <unordered_set>

#include <assert.h>

#include "zpr.h"
#include "utils.h"

namespace util
{
	struct timer
	{
		using hrc = std::chrono::high_resolution_clock;

		timer() : out(nullptr)              { start = hrc::now(); }
		explicit timer(double* t) : out(t)  { start = hrc::now(); }
		~timer()
		{
			if(out) *out = std::chrono::duration<double, std::micro>(hrc::now() - start).count();
		}
		double measure()
		{
			return std::chrono::duration<double, std::micro>(hrc::now() - start).count();
		}

		double* out = 0;
		std::chrono::time_point<hrc> start;
	};

	template <typename Container, typename T>
	bool contains(const Container& container, const T& x)
	{
		return container.find(x) != container.end();
	}

	struct str_view : public std::string_view
	{
		str_view()                          : std::string_view("") { }
		str_view(std::string&& s)           : std::string_view(std::move(s)) { }
		str_view(std::string_view&& s)      : std::string_view(std::move(s)) { }
		str_view(const std::string& s)      : std::string_view(s) { }
		str_view(const std::string_view& s) : std::string_view(s) { }
		str_view(const char* s)             : std::string_view(s) { }
		str_view(const char* s, size_t l)   : std::string_view(s, l) { }

		std::string_view sv() const   { return *this; }
		str_view drop(size_t n) const { return (this->size() >= n ? this->substr(n) : ""); }
		str_view take(size_t n) const { return (this->size() >= n ? this->substr(0, n) : *this); }
		str_view take_last(size_t n) const { return (this->size() >= n ? this->substr(this->size() - n) : *this); }
		str_view drop_last(size_t n) const { return (this->size() >= n ? this->substr(0, this->size() - n) : *this); }
		str_view substr(size_t pos = 0, size_t cnt = -1) const { return str_view(std::string_view::substr(pos, cnt)); }

		str_view& remove_prefix(size_t n) { std::string_view::remove_prefix(n); return *this; }
		str_view& remove_suffix(size_t n) { std::string_view::remove_suffix(n); return *this; }

		str_view trim_front(bool newlines = false) const
		{
			auto ret = *this;
			while(ret.size() > 0 && (ret[0] == ' ' || ret[0] == '\t' || (newlines && (ret[0] == '\r' || ret[0] == '\n'))))
				ret.remove_prefix(1);
			return ret;
		}
		str_view trim_back(bool newlines = false) const
		{
			auto ret = *this;
			while(ret.size() > 0 && (ret.back() == ' ' || ret.back() == '\t' || (newlines && (ret.back() == '\r' || ret.back() == '\n'))))
				ret.remove_suffix(1);
			return ret;
		}

		str_view trim(bool newlines = false) const
		{
			return this->trim_front(newlines).trim_back(newlines);
		}

		std::string str() const { return std::string(*this); }
	};

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

	static inline int64_t to_int(std::string_view s)
	{
		return std::stoll(std::string(s));
	}

	static inline std::optional<int64_t> try_int(std::string_view s)
	{
		try { return std::stoll(std::string(s)); }
		catch(...) { return { }; }
	}

	static inline str_view trim(str_view s)
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

	static inline std::string replace(std::string input, const std::string& thing, const std::string& with)
	{
		while(true)
		{
			if(auto it = input.find(thing); it != std::string::npos)
				input.replace(it, thing.size(), with);

			else
				break;
		}

		return input;
	}

	static inline std::vector<str_view> splitString(str_view view, char delim = '\n')
	{
		std::vector<str_view> ret;

		while(!view.empty())
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

	static inline std::vector<str_view> split(str_view str, str_view delim)
	{
		std::vector<str_view> parts;
		while(str.size() > 0)
		{
			auto idx = str.find(delim);
			if(idx == std::string::npos)
			{
				parts.push_back(str);
				break;
			}

			parts.push_back(str.take(idx));
			str = str.drop(idx + delim.size());
		}

		return parts;
	}

	static inline std::vector<str_view> split(str_view str, char delim)
	{
		return split(str, str_view(&delim, 1));
	}



	static inline std::vector<str_view> readFileLines(const std::string& path, char delim = '\n')
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


}

struct v2
{
	v2() : x(0), y(0) { }
	v2(int64_t x, int64_t y) : x(x), y(y) { }

	v2 up() const       { return v2(x, y - 1); }
	v2 down() const     { return v2(x, y + 1); }
	v2 left() const     { return v2(x - 1, y); }
	v2 right() const    { return v2(x + 1, y); }

	int64_t x;
	int64_t y;
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
	using value_type = int64_t;

	v3() : x(0), y(0), z(0) { }
	v3(v2 xy, value_type z) : x(xy.x), y(xy.y), z(z) { }
	v3(value_type x, value_type y, value_type z) : x(x), y(y), z(z) { }

	value_type x;
	value_type y;
	value_type z;

	v2 xx() const { return v2(x, x); }
	v2 yy() const { return v2(y, y); }
	v2 zz() const { return v2(z, z); }

	v2 xy() const { return v2(x, y); }
	v2 yx() const { return v2(y, x); }

	v2 xz() const { return v2(x, z); }
	v2 zx() const { return v2(z, x); }

	v2 zy() const { return v2(z, y); }
	v2 yz() const { return v2(y, z); }
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


struct v4
{
	using value_type = int64_t;

	v4() : x(0), y(0), z(0), w(0) { }
	v4(value_type x, value_type y, value_type z, value_type w) : x(x), y(y), z(z), w(w) { }

	value_type x;
	value_type y;
	value_type z;
	value_type w;
};

inline bool operator == (const v4& a, const v4& b) { return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w; }
inline bool operator != (const v4& a, const v4& b) { return !(a == b); }

inline bool operator < (const v4& a, const v4& b)
{
	return (std::vector<int64_t> { a.x, a.y, a.z, a.w }) < (std::vector<int64_t> { b.x, b.y, b.z, b.w });
}

inline bool operator > (const v4& a, const v4& b) { return b < a; }
inline bool operator <= (const v4& a, const v4& b) { return !(b < a); }
inline bool operator >= (const v4& a, const v4& b) { return !(a < b); }

inline v4 operator + (const v4& a, const v4& b) { return v4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
inline v4 operator - (const v4& a, const v4& b) { return v4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }

inline v4& operator += (v4& a, const v4& b) { a.x += b.x; a.y += b.y; a.z += b.z; a.w += b.w; return a; }
inline v4& operator -= (v4& a, const v4& b) { a.x -= b.x; a.y -= b.y; a.z -= b.z; a.w -= b.w; return a; }



namespace zpr
{
	template <>
	struct print_formatter<v2>
	{
		template <typename Cb>
		void print(const v2& v, Cb&& cb, format_args args)
		{
			cb("(");
			detail::print_one(static_cast<Cb&&>(cb), args, v.x);
			cb(", ");
			detail::print_one(static_cast<Cb&&>(cb), args, v.y);
			cb(")");
		}
	};

	template <>
	struct print_formatter<v3>
	{
		template <typename Cb>
		void print(const v3& v, Cb&& cb, format_args args)
		{
			cb("(");
			detail::print_one(static_cast<Cb&&>(cb), args, v.x);
			cb(", ");
			detail::print_one(static_cast<Cb&&>(cb), args, v.y);
			cb(", ");
			detail::print_one(static_cast<Cb&&>(cb), args, v.z);
			cb(")");
		}
	};
}

namespace std
{
	template <typename T>
	inline size_t hash_combine(size_t seed, const T& v)
	{
		return seed ^ hash<T>()(v) + 0x9E3779B97F4A7C15 + (seed << 6) + (seed >> 2);
	}

	template <>
	struct hash<v3>
	{
		size_t operator() (const v3& v) const
		{
			return hash_combine(hash_combine(
				(size_t) v.x,
				(size_t) v.y),
				(size_t) v.z);
		}
	};

	template <>
	struct hash<v4>
	{
		size_t operator() (const v4& v) const
		{
			return hash_combine(hash_combine(hash_combine(
				(size_t) v.x,
				(size_t) v.y),
				(size_t) v.z),
				(size_t) v.w);
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

	template <typename T>
	class grid
	{
		int64_t W;
		int64_t H;
		T* array;

	public:
		grid(int64_t w, int64_t h)
		{
			assert(w > 0 && h > 0);

			this->array = new T[w * h];
			this->W = w;
			this->H = h;
		}

		~grid()
		{
			delete[] this->array;
		}

		grid(const grid& g)
		{
			W = g.W;
			H = g.H;

			this->array = new T[W * H];
			for(int64_t i = 0; i < W * H; i++)
				this->array[i] = g.array[i];
		}

		grid& operator = (const grid& g)
		{
			if(this != &g)
			{
				delete[] this->array;

				W = g.W;
				H = g.H;
				this->array = new T[W * H];
				for(int64_t i = 0; i < W * H; i++)
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

		bool operator== (const grid& other) const
		{
			return W == other.W && H == other.H
				&& memcmp(this->array, other.array, sizeof(T) * W * H) == 0;
		}

		bool operator!= (const grid& other) const
		{
			return !(*this == other);
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

		T& operator() (int64_t x, int64_t y)
		{
			if(x < 0 || y < 0 || x >= W || y >= H)
				assert(!"out of bounds");
			return this->array[x + y * W];
		}

		T operator() (int64_t x, int64_t y) const
		{
			if(x < 0 || y < 0 || x >= W || y >= H)
				assert(!"out of bounds");
			return this->array[x + y * W];
		}

		T& operator [] (const v2& v)
		{
			return this->array[v.x + v.y * W];
		}

		const T& operator [] (const v2& v) const
		{
			return this->array[v.x + v.y * W];
		}

		bool in_bounds(const v2& v) const
		{
			return v.x >= 0 && v.y >= 0 && v.x < W && v.y < H;
		}

		// helpers
		size_t count(const T& x) const
		{
			size_t cnt = 0;
			for(int64_t i = 0; i < W * H; i++)
				if(this->array[i] == x) cnt++;

			return cnt;
		}

		template <typename U, typename Fn>
		grid<U> map(Fn&& f) const
		{
			auto ret = grid<U>(W, H);
			for(int64_t x = 0; x < W; x++)
				for(int64_t y = 0; y < H; y++)
					ret(x, y) = f(x, y, this->operator()(x, y));

			return ret;
		}

		template <typename Fn>
		void iter_xy(Fn&& fn) const
		{
			for(int64_t x = 0; x < W; x++)
				for(int64_t y = 0; y < H; y++)
					fn(x, y);
		}



		static inline grid<char> from_lines(const std::vector<str_view>& lines)
		{
			assert(!lines.empty());
			assert(!lines[0].empty());

			auto ret = grid<char>(lines[0].size(), lines.size());

			for(int64_t y = 0; y < ret.height(); y++)
				for(int64_t x = 0; x < ret.width(); x++)
					ret(x, y) = lines[y][x];

			return ret;
		}
	};
}











