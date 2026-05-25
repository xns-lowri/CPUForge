#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

enum class Endianness
{
	Little,
	Big
};

//isa register types
enum class RegisterType
{
	Data,
	Address,
	Control,
	Flag,
	Custom,
	Other
};

//instruction field types
enum class FieldType
{
	Opcode,
	Register,
	Immediate,
	Address,
	Flag,
	Label,
	Custom,
	Other
};

//#include "../project/Project.h"
#include "Instruction.h"

/* Structs defining individual data types in ISA */
struct ISADataType
{
	ItemHeader header;

	uint32_t bitWidth = 8;
	bool isSigned = false;

	std::vector<std::string> tags;
};

/* Structs defining registers available in ISA*/
struct ISARegister
{
	ItemHeader header;

	uint32_t bitWidth = 8;
	RegisterType type = RegisterType::Other;

	std::vector<std::string> aliases;
	std::vector<std::string> tags;
};

struct ISADefinitionDocument
{
	DocumentHeader header;

	Endianness endianness = Endianness::Little;

	uint32_t defaultDataWordSize = 8;	//in bits
	uint32_t defaultAddressWordSize = 8;	//in bits

	uint32_t maxInstructionWidth = 8;	//in bits - sets total number of instructions available
	uint32_t defaultOpcodeLength = 8;	//in bits

	//todo data types
	std::unordered_map<UUID, ISADataType> dataTypes;
	//todo registers
	std::unordered_map<UUID, ISARegister> registers;
	//instruction encodings allowed in isa
	std::unordered_map<UUID, InstructionEncoding> encodings;
	//instructions provided by isa
	std::unordered_map<UUID, InstructionDef> instructions;
};