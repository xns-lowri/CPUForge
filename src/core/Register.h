#pragma once
#include <string>
#include <vector>

#include "enumIsaDef.h"

/* Structs defining registers available in ISA*/
//struct IsaRegisterType
//{
//	UUID id = 0;
//	std::string name;
//	std::string description;
//};

struct IsaRegisterField {
	UUID id = 0;			
	std::string name;			
	std::string mnemonic;		
	std::string description;

	uint16_t bitWidth = 8;

	bool readable = true;
	bool writable = true;
};

struct IsaRegister
{
	UUID id = 0;			//generated id, track registers in app
	std::string name;			//human readable e.g. GP Data Register 0
	std::string mnemonic;		//string identifier e.g. r0, gpr0 - assy/disassy
	std::string description;	//write a novel if u want xo

	uint16_t bitWidth = 8;
	RegisterType type = RegisterType::Other;
	std::string customTypeString; //todo is this necessary?

	bool readable = true;
	bool writable = true;

	std::vector<IsaRegisterField> fields;

	std::vector<std::string> aliases;
	std::vector<std::string> tags;

	//todo link to hardware? or link fields to hardware signal/bus?
};

struct IsaRegisterFile
{
	UUID id = 0;
	std::string name;
	std::string mnemonic;
	std::string description;

	uint16_t defaultBitWidth = 8;

	std::map<std::string, IsaRegister> registers;
};