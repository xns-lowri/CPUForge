#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

#include "../../_types.h"
#include "../../project/Project.h"

#include "enumIsaDef.h"

#include "Instruction.h"
#include "Register.h"

//TODO remove UUID fields from all isa objects
//and generally from project data objects going forward??
//strict enforce unique string ids instead??

/* Structs defining individual data types in ISA */
struct IsaDataType
{
	UUID id = 0;
	std::string name;
	std::string description;

	uint16_t bitWidth = 8;
	bool isSigned = false;

	std::vector<std::string> tags;
};

struct IsaDefinitionDocument
{
	DocumentHeader header;

	Endianness endianness = Endianness::Little;

	uint16_t defaultDataWordSize = 8;	//in bits
	uint16_t defaultAddressWordSize = 8;	//in bits

	uint16_t maxInstructionWidth = 8;	//in bits - sets total number of instructions available
	uint16_t defaultOpcodeLength = 8;	//in bits

	//todo data types
	std::unordered_map<UUID, IsaDataType> dataTypes;
	//todo registers
	//std::unordered_map<UUID, ISARegister> registers;
	//instruction encodings allowed in isa
	std::unordered_map<UUID, InstructionEncoding> encodings;
	//instructions provided by isa
	std::unordered_map<UUID, InstructionDef> instructions;
};