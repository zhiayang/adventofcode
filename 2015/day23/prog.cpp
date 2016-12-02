// prog.cpp
// Copyright (c) 2014 - The Foreseeable Future, zhiayang@gmail.com
// Licensed under the Apache License Version 2.0.

// advent day 23

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




enum OPCODE
{
	HALF = 1,
	TRIPLE = 2,
	INCR = 3,
	JMPREL = 4,
	BREVEN = 5,
	BRONE = 6
};

struct Instruction
{
	OPCODE op;

	int arg = 0;
	int arg2 = 0;
};

struct VMState
{
	size_t ip = 0;

	size_t a = 1;
	size_t b = 0;

	std::vector<Instruction> instrs;
};


void fail(std::string line)
{
	printf("invalid input: %s\n", line.c_str());
	abort();
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




	VMState vm;
	for(auto line : strings)
	{
		Instruction in;

		if(line.find("hlf") == 0)
		{
			in.op = OPCODE::HALF;
			line = line.substr(strlen("hlf "));

			if(line[0] == 'a')			in.arg = 'a';
			else if(line[0] == 'b')		in.arg = 'b';
			else						fail(line);
		}
		else if(line.find("tpl") == 0)
		{
			in.op = OPCODE::TRIPLE;
			line = line.substr(strlen("tpl "));

			if(line[0] == 'a')			in.arg = 'a';
			else if(line[0] == 'b')		in.arg = 'b';
			else						fail(line);
		}
		else if(line.find("inc") == 0)
		{
			in.op = OPCODE::INCR;
			line = line.substr(strlen("inc "));

			if(line[0] == 'a')			in.arg = 'a';
			else if(line[0] == 'b')		in.arg = 'b';
			else						fail(line);
		}
		else if(line.find("jmp") == 0)
		{
			in.op = OPCODE::JMPREL;
			line = line.substr(strlen("jmp "));

			int ofs = std::stoi(line);
			in.arg = ofs;
		}
		else if(line.find("jie") == 0)
		{
			in.op = OPCODE::BREVEN;
			line = line.substr(strlen("jie "));

			if(line[0] == 'a')			in.arg = 'a';
			else if(line[0] == 'b')		in.arg = 'b';
			else						fail(line);

			line = line.substr(1);
			if(line[0] != ',')
				abort();

			line = line.substr(strlen(", "));

			int ofs = std::stoi(line);
			in.arg2 = ofs;
		}
		else if(line.find("jio") == 0)
		{
			in.op = OPCODE::BRONE;
			line = line.substr(strlen("jio "));

			if(line[0] == 'a')			in.arg = 'a';
			else if(line[0] == 'b')		in.arg = 'b';
			else						fail(line);

			line = line.substr(1);
			if(line[0] != ',')
				abort();

			line = line.substr(strlen(", "));

			int ofs = std::stoi(line);
			in.arg2 = ofs;
		}
		else
		{
			abort();
		}


		vm.instrs.push_back(in);
	}






	while(vm.ip < vm.instrs.size())
	{
		auto& instr = vm.instrs[vm.ip];
		switch(instr.op)
		{
			case HALF:
				if(instr.arg == 'a')	vm.a /= 2;
				else					vm.b /= 2;
				break;

			case TRIPLE:
				if(instr.arg == 'a')	vm.a *= 3;
				else					vm.b *= 3;
				break;

			case INCR:
				if(instr.arg == 'a')	vm.a += 1;
				else					vm.b += 1;
				break;

			case JMPREL:
				vm.ip += instr.arg;
				continue;

			case BREVEN:
				if(instr.arg == 'a')
				{
					if(vm.a % 2 == 0)
					{
						vm.ip += instr.arg2;
						continue;
					}
				}
				else
				{
					if(vm.b % 2 == 0)
					{
						vm.ip += instr.arg2;
						continue;
					}
				}
				break;

			case BRONE:
				if(instr.arg == 'a')
				{
					if(vm.a == 1)
					{
						vm.ip += instr.arg2;
						continue;
					}
				}
				else
				{
					if(vm.b == 1)
					{
						vm.ip += instr.arg2;
						continue;
					}
				}
				break;

			default:
				abort();
		}

		vm.ip++;
	}


	printf("a = %zu, b = %zu\n", vm.a, vm.b);












	fclose(f);
}







