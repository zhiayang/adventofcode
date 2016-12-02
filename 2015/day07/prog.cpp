// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 7

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include <map>
#include <vector>
#include <bitset>
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


size_t tonum(std::string s, size_t* index)
{
	size_t ret = 0;
	try
	{
		ret = std::stoi(s, index);
	}
	catch(std::exception)
	{
	}

	return ret;
}

int main()
{
	size_t size = 0;
	uint8_t* buf = 0;

	FILE* f = openInput(&size, &buf);



	std::string file((char*) buf);
	while(file.find("AND") != std::string::npos)
		file.replace(file.find("AND"), strlen("AND"), "&");

	while(file.find("OR") != std::string::npos)
		file.replace(file.find("OR"), strlen("OR"), "|");

	while(file.find("LSHIFT") != std::string::npos)
		file.replace(file.find("LSHIFT"), strlen("LSHIFT"), "<<");

	while(file.find("RSHIFT") != std::string::npos)
		file.replace(file.find("RSHIFT"), strlen("RSHIFT"), ">>");

	while(file.find("NOT") != std::string::npos)
		file.replace(file.find("NOT"), strlen("NOT"), "~");

	file.erase(std::remove(file.begin(), file.end(), ' '), file.end());


	std::vector<std::string> instrs;
	std::string cur = "";
	for(size_t i = 0; i < file.size(); i++)
	{
		if(file[i] != '\n' && file[i] != 0)
		{
			cur += file[i];
		}
		else
		{
			instrs.push_back(cur);
			cur = "";
		}
	}


	for(auto& instr : instrs)
	{
		size_t arrow = instr.find("->");
		std::string out = instr.substr(arrow + 2, -1);

		instr = out + "=" + instr.substr(0, arrow);

		// printf("%s\n", instr.c_str());
	}


	size_t dump = 0;
	// loop through
	again:

	std::string entire;
	for(auto instr : instrs)
	{
		// replace all instances of the thing on the left by the thing on the right
		size_t eq = instr.find("=");
		std::string tgt = instr.substr(0, eq);

		for(auto& ins : instrs)
		{
			size_t e = instr.find("=");
			std::string r = ins.substr(e + 1);

			while(r.find(tgt) != std::string::npos)
			{
				r.replace(r.find(tgt), tgt.size(), "(" + r + ")");
				ins = ins.substr(0, e) + "=" + r;
			}
		}

		if(tgt != "0" && tonum(tgt, &dump) == 0)
		{
			std::string rep = instr.substr(eq + 1);

			while(rep.find(tgt) != std::string::npos)
			{
				rep.replace(rep.find(tgt), tgt.size(), "(" + rep + ")");
				printf("replacing\n");
			}
			printf("done (replacing %s with %s)\n", tgt.c_str(), rep.c_str());
		}

		entire += instr + "\n";
	}

	// printf("%s\n", entire.c_str());

	auto test = [](char c) -> bool {
		return isalpha(c);
	};

	if(std::find_if(entire.begin(), entire.end(), test) != entire.end())
		goto again;


	fclose(f);
}








#if 0
struct Gate;

struct Wire
{
	bool init = false;

	uint16_t value = 0;
	std::vector<Gate*> connectedGates;
};

struct Connection
{
	Gate* input = 0;
	Wire* output = 0;
};

struct Gate
{
	// 1 = NOT
	// 2 = AND
	// 3 = OR
	// 4 = SHL
	// 5 = SHR
	// 6 = passthrough
	int kind = 0;
	uint16_t value = 0;

	Wire* input1 = 0;
	Wire* input2 = 0;
	Connection* connection = 0;

	void check()
	{
		bool pass = false;

		if(this->kind == 1)
		{
			if(this->input1->init)
			{
				this->value = ~this->input1->value;
				pass = true;
			}
		}
		else if(this->kind == 6)
		{
			if(this->input1->init)
			{
				this->value = this->input1->value;
				pass = true;
			}
		}
		else if(this->input1->init && this->input2->init)
		{
			switch(this->kind)
			{
				case 2:
					this->value = this->input1->value & this->input2->value;
					pass = true;
					break;

				case 3:
					this->value = this->input1->value | this->input2->value;
					pass = true;
					break;

				case 4:
					this->value = this->input1->value << this->input2->value;
					pass = true;
					break;

				case 5:
					this->value = this->input1->value >> this->input2->value;
					pass = true;
					break;
			}
		}

		if(pass)
		{
			this->connection->output->value = this->value;
		}
	}
};




























#define PART 1

int main()
{
	size_t size = 0;
	uint8_t* buf = 0;

	FILE* f = openInput(&size, &buf);



	std::vector<std::string> instrs;
	std::string cur = "";
	for(size_t i = 0; i < size; i++)
	{
		if(buf[i] != '\n' && buf[i] != 0)
		{
			cur += buf[i];
		}
		else
		{
			instrs.push_back(cur);
			cur = "";
		}
	}



	std::unordered_map<std::string, Wire*> wires;
	std::vector<Connection*> conns;

	size_t line = 1;
	for(auto instr : instrs)
	{
		bool notwire = false;
		if(instr.find("NOT ") == 0)
		{
			notwire = true;
			instr = instr.substr(strlen("NOT "));	// with space
		}


		int operation = 0;
		if(notwire) operation = 1;

		Connection* conn = new Connection();
		Gate* gate = new Gate();
		Wire* lhs = new Wire();
		Wire* rhs = new Wire();
		Wire* out = new Wire();

		// parse the input.
		if(!notwire)
		{
			uint16_t lval = 0;
			uint16_t rval = 0;

			if(isdigit(instr[0]))
			{
				size_t last = 0;
				lval = tonum(instr, &last);
				instr = instr.substr(std::to_string(lval).size());

				lhs->value = lval;
			}
			else
			{
				std::string wire = "";

				size_t i = 0;
				while(instr[i] != ' ' && i < instr.size())
				{
					wire += instr[i];
					i++;
				}

				if(wires[wire] != 0)
					lhs = wires[wire];

				else
					wires[wire] = lhs;

				instr = instr.substr(wire.size());
			}


			if(instr.find(" AND ") == 0)
			{
				operation = 2;
				instr = instr.substr(strlen(" AND "));
			}
			else if(instr.find(" OR ") == 0)
			{
				operation = 3;
				instr = instr.substr(strlen(" OR "));
			}
			else if(instr.find(" LSHIFT ") == 0)
			{
				operation = 4;
				instr = instr.substr(strlen(" LSHIFT "));
			}
			else if(instr.find(" RSHIFT ") == 0)
			{
				operation = 5;
				instr = instr.substr(strlen(" RSHIFT "));
			}
			else if(instr.find(" -> ") == 0)
			{
				operation = 6;
				instr = instr.substr(strlen(" -> "));
			}



			if(isdigit(instr[0]))
			{
				size_t last = 0;
				rval = tonum(instr, &last);
				instr = instr.substr(std::to_string(rval).size());

				rhs->value = rval;
			}
			else
			{
				std::string wire = "";

				size_t i = 0;
				while(instr[i] != ' ' && i < instr.size())
				{
					wire += instr[i];
					i++;
				}

				instr = instr.substr(wire.size());

				if(wires[wire] != 0)
					rhs = wires[wire];

				else
					wires[wire] = rhs;
			}
		}
		else
		{
			operation = 1;
			if(isdigit(instr[0]))
			{
				size_t last = 0;
				size_t input = ((uint16_t) tonum(instr, &last));
				instr = instr.substr(std::to_string(input).size());

				lhs->value = input;
			}
			else
			{
				std::string wire = "";

				size_t i = 0;
				while(instr[i] != ' ' && i < instr.size())
				{
					wire += instr[i];
					i++;
				}

				instr = instr.substr(wire.size());

				if(wires[wire] != 0)
					lhs = wires[wire];

				else
					wires[wire] = lhs;
			}
		}


		{
			if(instr.find(" -> ") != 0 && operation != 6)
			{
				fprintf(stderr, "invalid input: %s (line %zu)\n", instr.c_str(), line);
				exit(-1);
			}

			std::string output = operation == 6 ? instr : instr.substr(strlen(" -> "));
			if(wires[output] != 0)
				out = wires[output];

			else
				wires[output] = out;

			out->init = true;
		}


		lhs->connectedGates.push_back(gate);
		rhs->connectedGates.push_back(gate);
		out->connectedGates.push_back(gate);


		gate->kind = operation;
		gate->input1 = lhs;
		gate->input2 = rhs;
		gate->connection = conn;

		conn->input = gate;
		conn->output = out;

		conns.push_back(conn);

		line++;
	}

	for(auto conn : conns)
	{
		conn->input->check();
	}




	printf("a = %u\n", wires["a"]->value);






	fclose(f);
}
#endif






