// prog.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "aoc2.h"
using namespace util;

struct Passport
{
	str_view byr;
	str_view iyr;
	str_view eyr;
	str_view hgt;
	str_view hcl;
	str_view ecl;
	str_view pid;
	str_view cid;

	static bool checkNum(str_view str, int min, int max)
	{
		auto x = try_int(str);
		if(!x) return false;

		return min <= *x && *x <= max;
	}

	bool present() const
	{
		return !byr.empty() && !iyr.empty() && !eyr.empty()
			&& !hgt.empty() && !hcl.empty() && !ecl.empty()
			&& !pid.empty();
	}

	bool valid() const
	{
		if(!present())
			return false;

		if(!checkNum(byr, 1920, 2002) || !checkNum(iyr, 2010, 2020) || !checkNum(eyr, 2020, 2030))
			return false;

		bool heightOk = false;
		if(hgt.find("cm") != -1)
			heightOk = checkNum(hgt.drop_last(2), 150, 193);

		if(hgt.find("in") != -1)
			heightOk = checkNum(hgt.drop_last(2), 59, 76);

		if(!heightOk)
			return false;

		if(hcl.size() != 7 || hcl[0] != '#' || hcl.substr(1).find_first_not_of("0123456789abcdef") != -1)
			return false;

		if(!match(ecl, "amb", "blu", "brn", "gry", "grn", "hzl", "oth"))
			return false;

		if(pid.size() != 9 || !try_int(pid))
			return false;

		return true;
	}
};

int main()
{
	// fuck you, topaz.
	auto fixed = replace(replace(replace(readFile("input.txt"), "\n\n", "|"), "\n", " "), "|", "\n");
	auto lines = splitString(fixed, '\n');

	std::vector<Passport> passports;
	for(auto& line : lines)
	{
		Passport cur = { };
		auto parts = splitString(line, ' ');
		for(auto& part : parts)
		{
			if(part.find("byr") == 0)       cur.byr = part.drop(4);
			else if(part.find("iyr") == 0)  cur.iyr = part.drop(4);
			else if(part.find("eyr") == 0)  cur.eyr = part.drop(4);
			else if(part.find("hgt") == 0)  cur.hgt = part.drop(4);
			else if(part.find("hcl") == 0)  cur.hcl = part.drop(4);
			else if(part.find("ecl") == 0)  cur.ecl = part.drop(4);
			else if(part.find("pid") == 0)  cur.pid = part.drop(4);
			else if(part.find("cid") == 0)  cur.cid = part.drop(4);
		}

		passports.push_back(std::move(cur));
	}

	zpr::println("part 1 = {}", filter(passports, [](auto& p) -> bool { return p.present(); }).size());
	zpr::println("part 2 = {}", filter(passports, [](auto& p) -> bool { return p.valid(); }).size());
}









