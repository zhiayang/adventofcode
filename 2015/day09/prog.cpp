// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 9

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

FILE* openInput(size_t* size, uint8_t** buf)
{
	FILE* f = fopen("input", "r");
	if(f == 0)
	{
		fprintf(stderr, "file read error FILE* was null :: %d / %s\n", errno, strerror(errno));
		exit(-1);
	}

	struct stat st;
	fstat(fileno(f), &st);

	*size = st.st_size;
	*buf = new uint8_t[*size + 1];
	if(*buf == 0)
	{
		fprintf(stderr, "failed to allocate %zu bytes\n", *size + 1);
		exit(-1);
	}

	size_t readbytes = fread(*buf, 1, *size, f);
	if(readbytes != *size)
	{
		fprintf(stderr, "file read error (need %zu bytes, read %zu) :: %d / %s\n", *size, readbytes, errno, strerror(errno));
		exit(-1);
	}

	return f;
}

struct Path;
struct City
{
	std::string name;
	std::vector<Path*> edges;
};

struct Path
{
	City* src = 0;
	City* dst = 0;

	int dist = 0;
};

struct Graph
{
	std::unordered_map<std::string, City*> cities;
	std::vector<Path*> paths;
};

struct Route
{
	std::vector<City*> cities;
	size_t dist = 0;
	uint8_t visited = 0;

	// alpha = 1
	// arbre = 2
	// faerun = 4
	// norrath = 8
	// snowdin = 16
	// tambi = 32
	// tristram = 64
	// straylight = 128

	bool didvisit(std::string c)
	{
		if(c == "AlphaCentauri")
		{
			return this->visited & 0x01;
		}
		else if(c == "Arbre")
		{
			return this->visited & 0x02;
		}
		else if(c == "Faerun")
		{
			return this->visited & 0x04;
		}
		else if(c == "Norrath")
		{
			return this->visited & 0x08;
		}
		else if(c == "Snowdin")
		{
			return this->visited & 0x10;
		}
		else if(c == "Tambi")
		{
			return this->visited & 0x20;
		}
		else if(c == "Tristram")
		{
			return this->visited & 0x40;
		}
		else if(c == "Straylight")
		{
			return this->visited & 0x80;
		}
		else
		{
			abort();
		}
	}


	void visit(std::string c)
	{
		if(c == "AlphaCentauri")
		{
			this->visited |= 0x01;
		}
		else if(c == "Arbre")
		{
			this->visited |= 0x02;
		}
		else if(c == "Faerun")
		{
			this->visited |= 0x04;
		}
		else if(c == "Norrath")
		{
			this->visited |= 0x08;
		}
		else if(c == "Snowdin")
		{
			this->visited |= 0x10;
		}
		else if(c == "Tambi")
		{
			this->visited |= 0x20;
		}
		else if(c == "Tristram")
		{
			this->visited |= 0x40;
		}
		else if(c == "Straylight")
		{
			this->visited |= 0x80;
		}
		else
		{
			abort();
		}
	}


	bool operator == (const Route& other)
	{
		return this->cities == other.cities;
	}
};

static size_t count = 0;
std::vector<Route> search(std::vector<Path*> paths, Route* current, std::vector<Route> done, size_t num)
{
	std::vector<Route> ret = done;
	for(auto p : paths)
	{
		// printf("loop (%zu paths total)\n", paths.size());
		bool found = false;
		found = current->didvisit(p->dst->name);

		// for(auto c : current->cities)
		// {
		// 	if(c->name == p->dst->name)
		// 	{
		// 		found = true;
		// 		break;
		// 	}
		// }

		if(!found)
		{
			// printf("\r                                       \rpath (%s -> %s) : %zu", p->src->name.c_str(), p->dst->name.c_str(), ret.size());

			Route* n1 = new Route();
			n1->cities = current->cities;
			n1->cities.push_back(p->dst);
			n1->visited = current->visited;
			n1->visit(p->dst->name);

			n1->dist = current->dist + p->dist;
			// printf("dist = %zu\n", n1->dist);

			ret = search(p->dst->edges, n1, ret, num);
		}
		else
		{
			if(current->cities.size() == num && std::find(ret.begin(), ret.end(), *current) == ret.end())
			{
				ret.push_back(*current);

				count++;
				fprintf(stderr, "\r            \r%zu", count);
			}
		}
	}

	return ret;
}


Route search(Graph* g)
{
	Route best;

	std::vector<Route> all;
	for(auto c : g->cities)
	{
		Route* rt = new Route();
		rt->cities.push_back(c.second);
		rt->visit(c.first);

		auto rs = search(c.second->edges, rt, { }, g->cities.size());

		for(auto r : rs)
			all.push_back(r);
	}

	printf("\n%zu routes\n", all.size());
	for(auto r : all)
	{
		for(auto c : r.cities)
			printf("%s -> ", c->name.c_str());

		printf("%zu\n", r.dist);
	}


	return best;
}



























int main()
{
	size_t size = 0;
	uint8_t* buf = 0;

	FILE* f = openInput(&size, &buf);


	std::vector<std::string> strings;
	std::string cur = "";

	for(size_t i = 0; i < size; i++)
	{
		if(buf[i] != '\n' && buf[i] != 0)
		{
			cur += buf[i];
		}
		else
		{
			strings.push_back(cur);
			cur = "";
		}
	}



	Graph* graph = new Graph();

	for(auto s : strings)
	{
		std::string from = "";
		std::string to = "";
		std::string sdist = "";
		int dist = 0;

		int state = 0;
		for(size_t i = 0; i < s.length(); i++)
		{
			if(s[i] != ' ')
			{
				if(state == 0)
					from += s[i];

				else if(state == 1)
					to += s[i];

				else
					sdist += s[i];
			}
			else
			{
				if(s[i + 1] == 't')
				{
					s = s.substr(3); // "to "
					state = 1;
				}
				else if(s[i + 1] == '=')
				{
					s = s.substr(2); // "= "
					state = 2;
				}
			}
		}

		dist = std::stoi(sdist);

		City* src = graph->cities[from];
		if(src == 0)
		{
			src = new City();
			src->name = from;
			graph->cities[from] = src;
		}


		City* dst = graph->cities[to];
		if(dst == 0)
		{
			dst = new City();
			dst->name = to;
			graph->cities[to] = dst;
		}

		Path* pth = new Path();
		pth->src = src;
		pth->dst = dst;
		pth->dist = dist;

		src->edges.push_back(pth);

		Path* pth1 = new Path();
		pth1->src = dst;
		pth1->dst = src;
		pth1->dist = dist;
		dst->edges.push_back(pth1);


		printf("%s -> %s: %s\n", from.c_str(), to.c_str(), sdist.c_str());
	}

	search(graph);

	fclose(f);
}







