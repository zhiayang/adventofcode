// prog.cpp
// Copyright (c) 2018, zhiayang
// Licensed under the Apache License Version 2.0.

#include "assert.h"

#include <set>
#include <array>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "aoc.h"
#include "tinyformat.h"


struct wire_t;
struct gate_t
{
	std::string lhs;
	std::string rhs;

	std::string op;
};

struct wire_t
{
	std::string name;
	gate_t gate;

	bool solved = false;
	uint16_t value = 0;
};

uint16_t solve(std::unordered_map<std::string, wire_t>& wires, wire_t* wire);
uint16_t solve(std::unordered_map<std::string, wire_t>& wires, const std::string& v)
{
	if(isdigit(v[0]))
		return std::stoi(v);

	else
		return solve(wires, &wires[v]);
}

uint16_t solve(std::unordered_map<std::string, wire_t>& wires, wire_t* wire)
{
	if(wire->solved)
		return wire->value;

	uint16_t ret = 0;
	auto gate = wire->gate;
	{
		if(gate.op == "NOT")
			ret = ~solve(wires, gate.lhs);

		else if(gate.op == "->")
			ret = solve(wires, gate.lhs);

		else if(gate.op == "AND")
			ret = solve(wires, gate.lhs) & solve(wires, gate.rhs);

		else if(gate.op == "OR")
			ret = solve(wires, gate.lhs) | solve(wires, gate.rhs);

		else if(gate.op == "LSHIFT")
			ret = solve(wires, gate.lhs) << solve(wires, gate.rhs);

		else if(gate.op == "RSHIFT")
			ret = solve(wires, gate.lhs) >> solve(wires, gate.rhs);

		else
		{
			tfm::printfln("unknown op '%s' (wire %s)!", gate.op, wire->name);
			abort();
		}
	}

	wire->solved = true;
	wire->value = ret;

	return ret;
}


int main()
{
	auto lines = util::readFileLines("input.txt");
	std::unordered_map<std::string, wire_t> wires;

	int l = 0;
	for(const auto& line : lines)
	{
		if(line.find("NOT") == 0)
		{
			char a[16] = { 0 };
			char out[16] = { 0 };
			sscanf(line.c_str(), "NOT %s -> %s", &a, &out);

			gate_t gate;
			gate.op = "NOT";
			gate.lhs = std::string(a);

			wires[std::string(out)].name = out;
			wires[std::string(out)].gate = gate;
		}
		else if(line.find("AND") != std::string::npos || line.find("OR") != std::string::npos || line.find("SHIFT") != std::string::npos)
		{
			char a[16] = { 0 };
			char b[16] = { 0 };
			char op[16] = { 0 };
			char out[16] = { 0 };
			sscanf(line.c_str(), "%s %s %s -> %s", &a, &op, &b, &out);

			gate_t gate;
			gate.op = std::string(op);
			gate.lhs = std::string(a);
			gate.rhs = std::string(b);

			wires[std::string(out)].name = out;
			wires[std::string(out)].gate = gate;
		}
		else
		{
			char a[16] = { 0 };
			char out[16] = { 0 };
			sscanf(line.c_str(), "%s -> %s", &a, &out);

			gate_t gate;
			gate.op = "->";
			gate.lhs = std::string(a);

			wires[std::string(out)].name = out;
			wires[std::string(out)].gate = gate;
		}
	}




	uint16_t wire_a = 0;
	{
		auto wires_copy = wires;
		wire_a = solve(wires_copy, "a");

		tfm::printfln("part 1: value of wire a = %d", wire_a);
	}


	{
		auto wires_copy = wires;

		wires_copy["b"].solved = true;
		wires_copy["b"].value = wire_a;

		tfm::printfln("part 2: new value of wire a = %d", solve(wires_copy, "a"));
	}
}








