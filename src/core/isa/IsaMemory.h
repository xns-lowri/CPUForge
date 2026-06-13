#pragma once
#include <string>
#include <vector>
#include <optional>

#include "../../_types.h"
#include "../../project/Project.h"

#include "enumIsaDef.h"
#include "IsaContext.h"

//defines the rules and available formats of data for each memory access type
struct IsaMemoryAccessClass {
	std::string name;
	std::string friendlyName;
	std::string description;

	IsaMemoryAccessKind accessKind;

	std::vector<uint16_t>	legalAccessBitWidths;	//data output sizes
	std::optional<uint16_t> requiredAlignmentBits;	//data alignment

	IsaContextRule availablilityRule;

	std::vector<std::string> possibleFaults;

	std::vector<std::string> tags;
};
inline void to_json(json& j, const IsaMemoryAccessClass& access) {

}
inline void from_json(const json& j, IsaMemoryAccessClass& access) {

}
//todo to/from json


//logical memory regions within address space
struct IsaMemoryRegion {
	std::string name;
	std::string friendlyName;
	std::string description;

	uint64_t baseAddress;
	uint64_t sizeInAddressUnits;

	//region kind?

	std::vector<IsaMemoryAccessClass> accessClasses;

	//bool readable;
	//bool writable;
	//bool executable;

	std::optional<std::string> faultOnRead;
	std::optional<std::string> faultOnWrite;
	std::optional<std::string> faultOnExecute;

	std::vector<std::string> tags;
};
inline void to_json(json& j, const IsaMemoryRegion& region) {

}
inline void from_json(const json& j, IsaMemoryRegion& region) {

}

//todo to/from json


//physical address spaces in isa
struct IsaAddressSpace {
	std::string name;
	std::string friendlyName;
	std::string description;

	Endianness endianness = Endianness::Little;	//usually same as isa?

	//todo move to memory?
	IsaMemoryConsistencyModel memoryConsistencyModel
		= IsaMemoryConsistencyModel::Sequential;

	uint16_t addressWidthBits = 16;	//2^bits = number of units in address space

	IsaDataUnit dataUnitType = IsaDataUnit::Byte;	//byte units default
	uint16_t dataUnitWidthBits = 8;			//for custom width data

	//std::vector<uint16_t> legalAccessBitWidths; //data output sizes
	//uint16_t dataAlignmentWidthBits = 8;	//for alignment (byte-sizes?)
	//
	//bool readable;
	//bool writable;
	//bool executable;

	std::vector<IsaMemoryAccessClass> accessClasses;

	//all address spaces must have at least one memory region?
	//maybe not with access classes in space and regions
	std::vector<IsaMemoryRegion> memoryRegions;

	//translation stages/models - todo outside address space??

	std::vector<std::string> tags;
};
inline void to_json(json& j, const IsaAddressSpace& space) {

}
inline void from_json(const json& j, IsaAddressSpace& space) {

}

//todo to/from json

//struct IsaMemoryModel {

	//address spaces
	//translation models
	//memory mapped io?
//};