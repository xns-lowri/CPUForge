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

inline void from_json(const json& j, IsaDataType& type) {
	j.at("name").get_to(type.name);
	j.at("friendlyName").get_to(type.friendlyName);
	j.at("description").get_to(type.description);
	j.at("bitWidth").get_to(type.bitWidth);
	j.at("isSigned").get_to(type.isSigned);
	j.at("aliases").get_to(type.aliases);
	j.at("tags").get_to(type.tags);
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

inline void from_json(const json& j, IsaFeature& feature) {
	j.at("name").get_to(feature.name);
	j.at("friendlyName").get_to(feature.friendlyName);
	j.at("description").get_to(feature.description);
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
		{ "vectorAddress", fault.vectorAddress.value_or(-1) }
	};
}

inline void from_json(const json& j, IsaFault& fault) {
	j.at("name").get_to(fault.name);
	j.at("friendlyName").get_to(fault.friendlyName);
	j.at("description").get_to(fault.friendlyName);

	if (j.at("vectorName") == "none") { fault.vectorName = std::nullopt; }
	else { j.at("vectorName").get_to(fault.vectorName); }

	if (j.at("vectorAddress") == -1) { fault.vectorAddress = std::nullopt; }
	else { j.at("vectorAddress").get_to(fault.vectorAddress); }
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
	j.at("architectureName").get_to(def.architectureName);
	j.at("endianness").get_to<Endianness>(def.endianness);

	j.at("defaultDataWordSize").get_to(def.defaultDataWordSize);
	j.at("defaultAddressWordSize").get_to(def.defaultAddressWordSize);
	j.at("defaultOpcodeWidth").get_to(def.defaultOpcodeWidth);
	j.at("maxInstructionWidth").get_to(def.maxInstructionWidth);
	
	j.at("dataTypes").get_to(def.dataTypes);
	j.at("features").get_to(def.features);
	j.at("faults").get_to(def.faults);
	j.at("contextDimensions").get_to(def.contextDimensions);
	j.at("registerFiles").get_to(def.registerFiles);
	j.at("instructionFormats").get_to(def.instructionFormats);
	j.at("instructions").get_to(def.instructions);
}