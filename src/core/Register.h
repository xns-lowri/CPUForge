#pragma once
#include <string>
#include <vector>

#include "enumIsaDef.h"

/* Structs defining registers available in ISA*/
struct IsaRegisterType
{
	UUID id = 0;
	std::string name;
	std::string description;
};

struct IsaRegister
{
	UUID id = 0;			//generated id, track registers in app
	std::string name;			//human readable e.g. GP Data Register 0
	std::string mnemonic;		//string identifier e.g. r0, gpr0 - assy/disassy
	std::string description;	//write a novel if u want xo

	uint16_t bitWidth = 8;
	RegisterType type = RegisterType::Other;

	std::vector<std::string> aliases;
	std::vector<std::string> tags;
};

struct IsaRegisterFile
{
	UUID id = 0;
	std::string name;
	std::string mnemonic;
	std::string description;

	std::map<std::string, IsaRegister> registers;
};