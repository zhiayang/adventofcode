// prog.cpp
// Copyright (c) 2014 - 2017, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include <map>
#include <array>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "lexer.cpp"

int main()
{
	using TT = TokenType;
	auto tokens = getTokens("day08/input.txt");

	struct Instruction
	{
		std::string reg;
		bool incr = false;
		int val = 0;

		std::string condreg;
		int condval = 0;

		// -2 is <, -1 is <=, 0 is ==, 1 is >=, 2 is >, 3 is !=
		int cond = 0;
	};

	int local_max = INT_MIN;
	std::string local_mreg;

	int final_max = INT_MIN;
	std::string final_mreg;

	std::map<std::string, int> registers;
	// std::vector<Instruction> instructions;

	auto exec = [&registers](const Instruction& instr, int cond) {
		bool perform = false;
		std::string sc;
		if(cond == -2)		{ perform = (registers[instr.condreg] < instr.condval); sc = "<"; }
		else if(cond == -1)	{ perform = (registers[instr.condreg] <= instr.condval); sc = "<="; }
		else if(cond == 0)	{ perform = (registers[instr.condreg] == instr.condval); sc = "=="; }
		else if(cond == 1)	{ perform = (registers[instr.condreg] >= instr.condval); sc = ">="; }
		else if(cond == 2)	{ perform = (registers[instr.condreg] > instr.condval); sc = ">"; }
		else if(cond == 3)	{ perform = (registers[instr.condreg] != instr.condval); sc = "!="; }
		else				abort();

		if(perform)
		{
			printf("%s %s %d, do %s %s %d\n", instr.condreg.c_str(), sc.c_str(), instr.condval, instr.reg.c_str(),
				instr.incr ? "+" : "-", instr.val);
			registers[instr.reg] += (instr.incr ? 1 : -1) * (instr.val);
		}
	};

	int line = 1;
	for(size_t i = 0; i < tokens.size() && tokens[i] != TT::EndOfFile;)
	{
		if(tokens[i] == TT::NewLine)
		{
			i++;
			continue;
		}

		std::string name = tokens[i].str();

		Instruction instr;
		instr.reg = name;

		i++;
		if(tokens[i].str() == "inc")
			instr.incr = true;

		else
			instr.incr = false;

		i++;

		bool neg1 = false;
		if(tokens[i] == TT::Minus) { i++; neg1 = true; }
		assert(tokens[i] == TT::Number);

		instr.val = std::stoi(tokens[i].str());
		if(neg1) instr.val *= -1;

		i++;
		assert(tokens[i].str() == "if");
		i++;

		instr.condreg = tokens[i].str();
		i++;

		int op = -10;
		if(tokens[i] == TT::LAngle)
			op = -2;

		else if(tokens[i] == TT::LessThanEquals)
			op = -1;

		else if(tokens[i] == TT::EqualsTo)
			op = 0;

		else if(tokens[i] == TT::GreaterEquals)
			op = 1;

		else if(tokens[i] == TT::RAngle)
			op = 2;

		else if(tokens[i] == TT::NotEquals)
			op = 3;

		else
			assert(false && "invalid");

		instr.cond = op;

		i++;
		bool neg2 = false;
		if(tokens[i] == TT::Minus) { neg2 = true; i++; }
		instr.condval = std::stoi(tokens[i].str());
		if(neg2) { instr.condval *= -1; }

		i++;

		// instructions.push_back(instr);
		exec(instr, op);

		if(tokens[i] == TT::NewLine)
			i++;

		line++;

		for(const auto& reg : registers)
		{
			if(reg.second > local_max)
			{
				local_max = reg.second;
				local_mreg = reg.first;
			}
		}
	}

	for(const auto& reg : registers)
	{
		if(reg.second > final_max)
		{
			final_max = reg.second;
			final_mreg = reg.first;
		}
	}


	printf("local: %d, final: %d\n", local_max, final_max);
}









