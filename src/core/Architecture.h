#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

#include "../project/Project.h"
#include "enumIsaDef.h"
#include "Instruction.h"

/* Structs defining individual data types in ISA */
struct ISADataType
{
	UUID id = 0;
	std::string name;
	std::string description;

	uint16_t bitWidth = 8;
	bool isSigned = false;

	std::vector<std::string> tags;
};

/* Structs defining registers available in ISA*/
struct ISARegister
{
	UUID id = 0;
	std::string name;
	std::string description;

	uint16_t bitWidth = 8;
	RegisterType type = RegisterType::Other;

	std::vector<std::string> aliases;
	std::vector<std::string> tags;
};

struct ISADefinitionDocument
{
	DocumentHeader header;

	Endianness endianness = Endianness::Little;

	uint16_t defaultDataWordSize = 8;	//in bits
	uint16_t defaultAddressWordSize = 8;	//in bits

	uint16_t maxInstructionWidth = 8;	//in bits - sets total number of instructions available
	uint16_t defaultOpcodeLength = 8;	//in bits

	//todo data types
	std::unordered_map<UUID, ISADataType> dataTypes;
	//todo registers
	std::unordered_map<UUID, ISARegister> registers;
	//instruction encodings allowed in isa
	std::unordered_map<UUID, InstructionEncoding> encodings;
	//instructions provided by isa
	std::unordered_map<UUID, InstructionDef> instructions;
};