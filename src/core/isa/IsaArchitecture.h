#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

#include "../../_types.h"
#include "../../project/Project.h"

#include "enumIsaDef.h"
#include "IsaContext.h"
#include "IsaInstruction.h"
#include "IsaRegister.h"

//TODO remove UUID fields from all isa objects
//and generally from project data objects going forward??
//strict enforce unique string ids instead??

/* Structs defining individual data types in ISA */
struct IsaDataType
{
	//UUID id = 0;
	std::string name;
	std::string friendlyName;
	std::string description;

	uint16_t bitWidth = 8;
	bool isSigned = false;	
	//bool isCoded = false;	//e.g. float, bcd, char?

	std::vector<std::string> aliases;	//alternate names
	std::vector<std::string> tags;
};

inline void to_json(json& j, const IsaDataType& type) {
	j = json{
		{ "name", type.name },
		{ "friendlyName", type.friendlyName },
		{ "description", type.description },
		{ "bitWidth", type.bitWidth },
		{ "isSigned", type.isSigned },
		{ "aliases", type.aliases },
		{ "tags", type.tags }
	};
}

//potentially use for optionally tagging suppoprted ISA extensions?
struct IsaFeature
{
	std::string name;          // "mul", "interrupts", "mmu", "debug"
	std::string friendlyName;
	std::string description;
};

inline void to_json(json& j, const IsaFeature& feature) {
	j = json{
		{ "name", feature.name },
		{ "friendlyName", feature.friendlyName },
		{ "description", feature.description }
	};
}

/* Defines all potential faults that may be thrown in the ISA design */
struct IsaFault
{
	std::string name;	// "illegal_instruction", "privilege_violation"
	std::string friendlyName;
	std::string description;

	std::optional<std::string> vectorName;
	std::optional<uint64_t> vectorAddress;
};

inline void to_json(json& j, const IsaFault& fault) {
	j = json{
		{ "name", fault.name },
		{ "friendlyName", fault.friendlyName },
		{ "description", fault.description },
		{ "vectorName", fault.vectorName.value_or("none") },
		{ "vectorAddress", fault.vectorAddress.value_or(0) }
	};
}

struct IsaDefinition
{
	//generic header metadata, UUID id + string name
	DocumentHeader header;

	//isa metadata
	std::string architectureName = "";

	//isa definition
	Endianness endianness = Endianness::Little;

	uint16_t defaultDataWordSize = 8;	//in bits
	uint16_t defaultAddressWordSize = 8;	//in bits

	uint16_t defaultOpcodeWidth = 8;	//in bits
	uint16_t maxInstructionWidth = 8;	//in bits - sets total number of instructions available

	//isa components
	std::unordered_map<std::string, IsaDataType> dataTypes;	//data types supported
	std::vector<IsaFeature> features;	//features comprising ISA family
	std::vector<IsaFault> faults;

	std::vector<IsaContextDimension> contextDimensions;	//context dimensions inmplemented in ISA
	
	std::unordered_map<std::string, IsaRegisterFile> registerFiles;

	std::unordered_map<std::string, InstructionEncodingFormat> instructionFormats;
	std::unordered_map<std::string, IsaInstruction> instructions;

	//assembly syntax
	//context links
};

inline void to_json(json& j, const IsaDefinition& def) {
	j = json{
		{ "header", def.header },
		{ "architectureName", def.architectureName },
		{ "endianness", ToString(def.endianness) },
		{ "defaultDataWordSize", def.defaultDataWordSize },
		{ "defaultAddressWordSize", def.defaultAddressWordSize },
		{ "defaultOpcodeWidth", def.defaultOpcodeWidth },
		{ "maxInstructionWidth", def.maxInstructionWidth },
		{ "dataTypes", def.dataTypes },
		{ "features", def.features },
		{ "faults", def.faults },
		{ "contextDimensions", def.contextDimensions },
		{ "registerFiles", def.registerFiles },
		{ "instructionFormats", def.instructionFormats },
		{ "instructions", def.instructions }
	};
}

inline void from_json(const json& j, IsaDefinition& def) {
	j.at("header").get_to<DocumentHeader>(def.header);
}