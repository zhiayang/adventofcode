// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 13

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <map>
#include <deque>
#include <thread>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>


#if 0
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
struct Person
{
	int id = 0;
	std::vector<Path*> edges;
};

struct Path
{
	Person* src = 0;
	Person* dst = 0;

	ssize_t dist = 0;
};

struct Graph
{
	std::unordered_map<std::string, Person*> people;
	std::vector<Path*> paths;
};


// in order is clockwise.
struct Route
{
	std::deque<Person*> people;
	ssize_t dist = 0;
	ssize_t rdist = 0;
	uint16_t visited = 0;

	bool finished = 0;

	// alice = 1
	// bob = 2
	// carol = 4
	// david = 8
	// eric = 16
	// frank = 32
	// george = 64
	// mallory = 128

	uint16_t getFlag(int id) const
	{
		return 1 << (id - 1);
	}

	bool didvisit(int id) const
	{
		return this->visited & getFlag(id);
	}

	void visit(int id)
	{
		this->visited |= getFlag(id);
	}


	bool operator == (const Route& other)
	{
		return this->people == other.people;
	}
};

static int getid(std::string n)
{
	if(n == "Alice") return 1;
	if(n == "Bob") return 2;
	if(n == "Carol") return 3;
	if(n == "David") return 4;
	if(n == "Eric") return 5;
	if(n == "Frank") return 6;
	if(n == "George") return 7;
	if(n == "Mallory") return 8;
	if(n == "Self") return 9;

	return 0;
}

static size_t count = 0;
std::deque<Route> search(const std::vector<Path*>& paths, const Route& current, const std::deque<Route>& done, size_t num)
{
	std::deque<Route> ret = done;
	for(auto p : paths)
	{
		// printf("loop (%zu paths total)\n", paths.size());
		bool found = false;
		found = current.didvisit(p->dst->id);
		if(current.people.size() == num && p->dst == current.people.front())
			found = false;


		if(!found)
		{
			Route n1; // = new Route();
			n1.people = current.people;
			n1.people.push_back(p->dst);
			n1.visited = current.visited;
			n1.finished = current.finished;

			// printf("visit %s\n", p->dst->name.c_str());

			if(p->dst == n1.people.front() && !n1.finished)
				n1.finished = true;

			else if(n1.finished || p->dst != n1.people.front())
				n1.visit(p->dst->id);

			n1.dist = current.dist + p->dist;
			ret = search(p->dst->edges, n1, ret, num);
		}
		else
		{
			if(current.people.size() == num + 1/*&& std::find(ret.begin(), ret.end(), *current) == ret.end()*/)
			{
				ret.push_back(current);

				count++;
				// printf("<< done >>\n");
				fprintf(stderr, "\r            \r%zu", count);
			}
		}
	}

	return ret;
}



void search(Graph* g)
{
	std::vector<std::thread*> threads;
	std::vector<Route> all;

	for(auto c : g->people)
	{
		// Route* rt = new Route();
		Route rt;
		rt.people.push_back(c.second);
		rt.visit(c.second->id);

		auto rs = search(c.second->edges, rt, { }, g->people.size());

		for(auto r : rs)
			all.push_back(r);
	}


	for(auto& r : all)
	{
		Route tmp;

		for(size_t i = r.people.size() - 1; i > 0; i--)
		{
			Person* p = r.people[i];
			ssize_t dist = 0;

			for(auto pth : p->edges)
			{
				if(i >= 1 && pth->src == p && pth->dst == r.people[i - 1])
				{
					dist += pth->dist;
					tmp.visit(pth->dst->id);

					// printf("v %s\n", pth->dst->name.c_str());
					goto out;
				}
			}

			out:
				r.rdist += dist;
		}

		fprintf(stderr, "\r            \r%zu", count);
		count--;
	}



	ssize_t best = 0;
	printf("\n%zu routes\n", all.size());
	for(auto r : all)
	{
		for(auto c : r.people)
			printf("%d -> ", c->id);

		printf("%zd (%zd) // (%zd)\n", r.dist + r.rdist, r.dist, r.rdist);

		if(r.dist + r.rdist > best)
			best = r.dist + r.rdist;
	}

	printf("best = %zd\n", best);


	// return best;
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
		ssize_t dist = 0;

		bool neg = false;
		int state = 0;

		while(s.length() > 0)
		{
			if(s[0] != ' ')
			{
				if(state == 0)
					from += s[0];

				else if(state == 1)
					sdist += s[0];

				else
					to += s[0];

				s = s.substr(1);
			}
			else
			{
				// printf("s = %s\n", s.c_str());
				s = s.substr(1);

				if(s[0] == 'w')
				{
					s = s.substr(6); // "would "

					// printf("%s\n", s.c_str());
					if(s.find("lose") == 0)
						neg = true;

					s = s.substr(5); // "gain " or "lose "
					// printf("(1) : %s\n", s.c_str());

					state = 1;
				}
				else if(s[0] == 'h')
				{
					s = s.substr(35); // "happiness units by sitting next to "
					state = 2;
				}
			}
		}

		// printf("sdist = %s\n", sdist.c_str());
		dist = std::stoi(sdist);
		dist = (neg ? -dist : dist);

		Person* src = graph->people[from];
		if(src == 0)
		{
			src = new Person();
			src->id = getid(from);
			graph->people[from] = src;
		}


		Person* dst = graph->people[to];
		if(dst == 0)
		{
			dst = new Person();
			dst->id = getid(to);
			graph->people[to] = dst;
		}

		Path* pth = new Path();
		pth->src = src;
		pth->dst = dst;
		pth->dist = dist;

		src->edges.push_back(pth);

		printf("%s -> %s: %zd\n", from.c_str(), to.c_str(), dist);
	}

	search(graph);

	fclose(f);
}
#endif


// not mine

#include <regex>
#include <fstream>
#include <algorithm>

int main(int argc, char* argv[])
{
    std::fstream fs(argv[1]);
    std::map<std::string, std::map<std::string, int>> happinesses;

    std::string s;
    while (std::getline(fs, s)){
        std::smatch m;
        std::regex_match(s, m, std::regex("(.*) would (.*?) (.*?) .* (.*)\\."));
        happinesses[m[1]][m[4]] = (m[2] == "lose" ? -1 : 1) * std::stoi(m[3]);
        happinesses[m[1]]["Me"] = happinesses["Me"][m[4]] = 0;
    }

    std::vector<std::string> people(happinesses.size());
    std::transform(happinesses.begin(), happinesses.end(), people.begin(), [](auto p){return p.first;});

    int maxHapp = 0;
    do{
        int currHapp = (happinesses[*(people.end() - 1)][*people.begin()] +
                           happinesses[*people.begin()][*(people.end() - 1)]);

        for (auto it = people.begin(); it != (people.end() - 1); ++it)
            currHapp += (happinesses[*it][*(it + 1)] + happinesses[*(it + 1)][*it]);

        maxHapp = std::max(currHapp, maxHapp);
    }while (std::next_permutation(people.begin(), people.end()));

    printf("%d\n", maxHapp);
}






