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
#include "IsaMemory.h"
#include "IsaAddress.h"

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

	IsaDataTypeKind dataTypeKind = IsaDataTypeKind::Other;
	bool isSigned = false;	
	//bool isCoded = false;	//e.g. float, bcd, char?

	//todo add optional address space id for pointer types?

	std::vector<std::string> aliases;	//alternate names
	std::vector<std::string> tags;
};

inline void to_json(json& j, const IsaDataType& type) {
	j = json{
		{ "name", type.name },
		{ "friendlyName", type.friendlyName },
		{ "description", type.description },

		{ "dataTypeKind", ToString(type.dataTypeKind) },
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

	j.at("dataTypeKind").get_to<IsaDataTypeKind>(type.dataTypeKind);
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

	uint16_t defaultByteWidth = 8;

	bool dataUsesDefaultByte = true;
	bool addressUsesDefaultByte = true;
	bool instructionUsesDefaultByte = true;

	uint16_t defaultDataByteWidth = 8;
	uint16_t defaultDataWordBytes = 1;

	uint16_t defaultAddressByteWidth = 8;
	uint16_t defaultAddressWordBytes = 1;

	uint16_t defaultInstructionByteWidth = 8;
	uint16_t defaultInstructionWordBytes = 1;
	uint16_t maxInstructionWordBytes = 1;

	uint16_t defaultOpcodeWidth = 8;	
	uint16_t maxOpcodeWidth = 8;		//sets total number of instructions available

	uint16_t defaultAlignmentBits = 8;	//for memory addressing

	//isa components
	std::vector<IsaFeature> features;
	std::vector<IsaFault> faults;
	std::vector<IsaContextDimension> contextDimensions;

	std::unordered_map<std::string, IsaDataType> dataTypes;
	
	//register model:
	std::unordered_map<std::string, IsaRegisterFile> registerFiles;

	//todo address spaces/memory model
	std::unordered_map<std::string, IsaAddressSpace> addressSpaces;
	std::unordered_map<std::string, IsaAddressTranslationStage> addressTranslationStages;
	//todo ports 

	std::unordered_map<std::string, IsaInstructionEncodingFormat> instructionFormats;
	std::unordered_map<std::string, IsaInstruction> instructions;

	//assembly syntax
	//context links

	//todo think about how to include memory model:
	/*
	* Byte-addressability (memory access rules)
	* 
	* Memory modes:
	* - Immediate (value in instruction)
	* - Register Direct (value in register)
	* - Register Indirect (value in memory pointed to by register)
	* - Memory Direct (memory pointer in instruction)
	* - Memory Indirect (pointer from register, offset from instruction)
	*/
};

inline void to_json(json& j, const IsaDefinition& def) {
	j = json{
		{ "header", def.header },
		{ "architectureName", def.architectureName },
		{ "endianness", ToString(def.endianness) },

		{ "defaultByteWidth", def.defaultByteWidth },

		{ "dataUsesDefaultByte", def.dataUsesDefaultByte },
		{ "addressUsesDefaultByte", def.addressUsesDefaultByte },
		{ "instructionUsesDefaultByte", def.instructionUsesDefaultByte },

		{ "defaultDataByteWidth", def.defaultDataByteWidth },
		{ "defaultDataWordBytes", def.defaultDataWordBytes },

		{ "defaultAddressByteWidth", def.defaultAddressByteWidth },
		{ "defaultAddressWordBytes", def.defaultAddressWordBytes },

		{ "defaultInstructionByteWidth", def.defaultInstructionByteWidth },
		{ "defaultInstructionWordBytes", def.defaultInstructionWordBytes },
		{ "maxInstructionWordBytes", def.maxInstructionWordBytes },

		{ "defaultOpcodeWidth", def.defaultOpcodeWidth },
		{ "maxOpcodeWidth", def.maxOpcodeWidth },

		{ "defaultAlignmentBits", def.defaultAlignmentBits },

		//{ "memoryConsistencyModel", ToString(def.memoryConsistencyModel) },

		{ "features", def.features },
		{ "faults", def.faults },
		{ "contextDimensions", def.contextDimensions },

		{ "dataTypes", def.dataTypes },
		{ "registerFiles", def.registerFiles },

		{ "addressSpaces", def.addressSpaces },
		{ "addressTranslationStages", def.addressTranslationStages },

		{ "instructionFormats", def.instructionFormats },
		{ "instructions", def.instructions }
	};
}
inline void from_json(const json& j, IsaDefinition& def) {
	j.at("header").get_to<DocumentHeader>(def.header);
	j.at("architectureName").get_to(def.architectureName);
	j.at("endianness").get_to<Endianness>(def.endianness);

	j.at("defaultByteWidth").get_to(def.defaultByteWidth);

	j.at("dataUsesDefaultByte").get_to(def.dataUsesDefaultByte);
	j.at("addressUsesDefaultByte").get_to(def.addressUsesDefaultByte);
	j.at("instructionUsesDefaultByte").get_to(def.instructionUsesDefaultByte);

	j.at("defaultDataByteWidth").get_to(def.defaultDataByteWidth);
	j.at("defaultDataWordBytes").get_to(def.defaultDataWordBytes);

	j.at("defaultAddressByteWidth").get_to(def.defaultAddressByteWidth);
	j.at("defaultAddressWordBytes").get_to(def.defaultAddressWordBytes);

	j.at("defaultInstructionByteWidth").get_to(def.defaultInstructionByteWidth);
	j.at("defaultInstructionWordBytes").get_to(def.defaultInstructionWordBytes);
	j.at("maxInstructionWordBytes").get_to(def.maxInstructionWordBytes);

	j.at("defaultOpcodeWidth").get_to(def.defaultOpcodeWidth);
	j.at("maxOpcodeWidth").get_to(def.maxOpcodeWidth);

	j.at("defaultAlignmentBits").get_to(def.defaultAlignmentBits);

	//j.at("memoryConsistencyModel").get_to<IsaMemoryConsistencyModel>(def.memoryConsistencyModel);
	
	j.at("features").get_to(def.features);
	j.at("faults").get_to(def.faults);
	j.at("contextDimensions").get_to(def.contextDimensions);

	j.at("dataTypes").get_to(def.dataTypes);
	j.at("registerFiles").get_to(def.registerFiles);

	j.at("addressSpaces").get_to(def.addressSpaces);
	j.at("addressTranslationStages").get_to(def.addressTranslationStages);

	j.at("instructionFormats").get_to(def.instructionFormats);
	j.at("instructions").get_to(def.instructions);
}