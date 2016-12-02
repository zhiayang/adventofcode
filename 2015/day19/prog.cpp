// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 19

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include <map>
#include <array>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

// arrange the input manually.

struct Repl
{
	Repl() : from(""), to("") { }
	Repl(const Repl& r) : from(r.from), to(r.to) { }
	Repl(std::string f, std::string t) : from(f), to(t) { }

	std::string from;
	std::string to;
};

int main()
{
	std::unordered_map<std::string, std::vector<Repl>> replacements;
	std::vector<Repl> vrepls;

	replacements["Al"].push_back(Repl("Al", "ThF"));
	replacements["Al"].push_back(Repl("Al", "ThRnFAr"));
	replacements["B"].push_back(Repl("B", "BCa"));
	replacements["B"].push_back(Repl("B", "TiB"));
	replacements["B"].push_back(Repl("B", "TiRnFAr"));
	replacements["Ca"].push_back(Repl("Ca", "CaCa"));
	replacements["Ca"].push_back(Repl("Ca", "PB"));
	replacements["Ca"].push_back(Repl("Ca", "PRnFAr"));
	replacements["Ca"].push_back(Repl("Ca", "SiRnFYFAr"));
	replacements["Ca"].push_back(Repl("Ca", "SiRnMgAr"));
	replacements["Ca"].push_back(Repl("Ca", "SiTh"));
	replacements["F"].push_back(Repl("F", "CaF"));
	replacements["F"].push_back(Repl("F", "PMg"));
	replacements["F"].push_back(Repl("F", "SiAl"));
	replacements["H"].push_back(Repl("H", "CRnAlAr"));
	replacements["H"].push_back(Repl("H", "CRnFYFYFAr"));
	replacements["H"].push_back(Repl("H", "CRnFYMgAr"));
	replacements["H"].push_back(Repl("H", "CRnMgYFAr"));
	replacements["H"].push_back(Repl("H", "HCa"));
	replacements["H"].push_back(Repl("H", "NRnFYFAr"));
	replacements["H"].push_back(Repl("H", "NRnMgAr"));
	replacements["H"].push_back(Repl("H", "NTh"));
	replacements["H"].push_back(Repl("H", "OB"));
	replacements["H"].push_back(Repl("H", "ORnFAr"));
	replacements["Mg"].push_back(Repl("Mg", "BF"));
	replacements["Mg"].push_back(Repl("Mg", "TiMg"));
	replacements["N"].push_back(Repl("N", "CRnFAr"));
	replacements["N"].push_back(Repl("N", "HSi"));
	replacements["O"].push_back(Repl("O", "CRnFYFAr"));
	replacements["O"].push_back(Repl("O", "CRnMgAr"));
	replacements["O"].push_back(Repl("O", "HP"));
	replacements["O"].push_back(Repl("O", "NRnFAr"));
	replacements["O"].push_back(Repl("O", "OTi"));
	replacements["P"].push_back(Repl("P", "CaP"));
	replacements["P"].push_back(Repl("P", "PTi"));
	replacements["P"].push_back(Repl("P", "SiRnFAr"));
	replacements["Si"].push_back(Repl("Si", "CaSi"));
	replacements["Th"].push_back(Repl("Th", "ThCa"));
	replacements["Ti"].push_back(Repl("Ti", "BP"));
	replacements["Ti"].push_back(Repl("Ti", "TiTi"));


	for(auto r : replacements)
	{
		for(auto p : r.second)
			vrepls.push_back(p);
	}



	auto combine = [](std::vector<std::string> v) -> std::string {

		std::string ret;
		for(auto s : v)
			ret += s;

		return ret;
	};






	std::vector<std::string> input;
	input.push_back("C");
	input.push_back("Rn");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("B");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("Ti");
	input.push_back("B");
	input.push_back("P");
	input.push_back("Ti");
	input.push_back("Ti");
	input.push_back("B");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("P");
	input.push_back("B");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Th");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("Ti");
	input.push_back("B");
	input.push_back("P");
	input.push_back("B");
	input.push_back("P");
	input.push_back("Mg");
	input.push_back("Ar");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("Ti");
	input.push_back("Mg");
	input.push_back("Ar");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Th");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("Rn");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("Ti");
	input.push_back("Ti");
	input.push_back("B");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("Si");
	input.push_back("Th");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("Mg");
	input.push_back("Ar");
	input.push_back("F");
	input.push_back("Y");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("F");
	input.push_back("Y");
	input.push_back("Ca");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("Si");
	input.push_back("Th");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Th");
	input.push_back("P");
	input.push_back("B");
	input.push_back("P");
	input.push_back("Ti");
	input.push_back("Mg");
	input.push_back("Ar");
	input.push_back("Ca");
	input.push_back("P");
	input.push_back("Rn");
	input.push_back("Si");
	input.push_back("Al");
	input.push_back("Ar");
	input.push_back("P");
	input.push_back("B");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("F");
	input.push_back("Y");
	input.push_back("Si");
	input.push_back("Th");
	input.push_back("Ca");
	input.push_back("Rn");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("Ar");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("P");
	input.push_back("B");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("Mg");
	input.push_back("Y");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Th");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Al");
	input.push_back("Ar");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("P");
	input.push_back("B");
	input.push_back("Si");
	input.push_back("Al");
	input.push_back("Ar");
	input.push_back("B");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Th");
	input.push_back("Ca");
	input.push_back("P");
	input.push_back("B");
	input.push_back("Si");
	input.push_back("Th");
	input.push_back("P");
	input.push_back("B");
	input.push_back("P");
	input.push_back("B");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("F");
	input.push_back("Y");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("Si");
	input.push_back("Th");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("B");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("F");
	input.push_back("Y");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("Si");
	input.push_back("Th");
	input.push_back("Ca");
	input.push_back("P");
	input.push_back("B");
	input.push_back("Si");
	input.push_back("Th");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("P");
	input.push_back("Mg");
	input.push_back("Ar");
	input.push_back("Rn");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("P");
	input.push_back("Ti");
	input.push_back("B");
	input.push_back("Ca");
	input.push_back("P");
	input.push_back("Rn");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("F");
	input.push_back("Y");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("B");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Th");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("Th");
	input.push_back("Si");
	input.push_back("Th");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("Ti");
	input.push_back("Rn");
	input.push_back("P");
	input.push_back("Mg");
	input.push_back("Ar");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Th");
	input.push_back("Ca");
	input.push_back("P");
	input.push_back("B");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("B");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("P");
	input.push_back("Rn");
	input.push_back("Ca");
	input.push_back("Ca");
	input.push_back("P");
	input.push_back("Mg");
	input.push_back("Ar");
	input.push_back("Si");
	input.push_back("Rn");
	input.push_back("F");
	input.push_back("Y");
	input.push_back("F");
	input.push_back("Ar");
	input.push_back("Ca");
	input.push_back("Si");
	input.push_back("Th");
	input.push_back("Rn");
	input.push_back("P");
	input.push_back("B");
	input.push_back("P");
	input.push_back("Mg");
	input.push_back("Ar");




	std::unordered_map<std::string, int> uniqued;

	size_t r = 0;
	for(size_t i = 0; i < input.size(); i++)
	{
		if(replacements[input[i]].size() > 0)
		{
			for(auto rpl : replacements[input[i]])
			{
				std::vector<std::string> copy = input;
				copy[i] = rpl.to;

				uniqued[combine(copy)] += 1;
				fprintf(stderr, "\r                                        \rperformed %zu replacements", r++);
			}
		}
	}


	printf("\n");
	printf("%zu unique replacements\n", uniqued.size());




	// do part 2.
	std::string comb = combine(input);

	std::function<void (std::string*, std::vector<Repl>, int)> reverse;


	std::vector<int> steplist;

	reverse = [&reverse, &steplist](std::string* input, std::vector<Repl> reps, int steps) -> void {

		static int rec = 0;
		if(*input == "e")
		{
			steplist.push_back(steps);
			return;
		}

		for(auto rep : reps)
		{
			if(size_t i = input->find(rep.to) != std::string::npos)
			{
				std::string* copy = new std::string(*input);

				copy->replace(i, rep.to.length(), rep.from);
				fprintf(stderr, "\r          \r%d", rec++);
				reverse(copy, reps, steps + 1);

				delete copy;
			}
		}
	};

	reverse(&comb, vrepls, 0);

	int min = *std::min_element(steplist.begin(), steplist.end());
	printf("min = %d\n", min);

	// part 2 fail
}












