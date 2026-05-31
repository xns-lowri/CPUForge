#pragma once
#include <string>
#include <vector>

#include "../../_types.h"
#include "../../project/Project.h"

#include "enumIsaDef.h"

/* Structs defining registers available in ISA*/
//struct IsaRegisterType
//{
//	UUID id = 0;
//	std::string name;
//	std::string description;
//};

/* Custom enum definitions for register field values */
struct IsaEnumValue
{
	//UUID id = 0;
	std::string name;
	std::string friendlyName;
	uint64_t value;
	std::string description;
};

/* Defines individual fields within register 
* - gp registers may have 1 field: r0(16) has data(16)
* - special registers may have more: FLAGS has PRV(2), Z/N/C/V(1), etc
*/
struct IsaRegisterField {
	//UUID id = 0;			
	std::string name;			
	std::string friendlyName;		
	std::string description;

	uint16_t startOffset = 0;
	uint16_t bitWidth = 8;

	bool readable = true;
	bool writable = true;

	std::optional<uint64_t> resetValue;	//todo hardware??

	std::vector<IsaEnumValue> enumValues;

	//todo link from hardware side?
	//std::optional<std::string> linkedHardware;
};

/* Define single register within design */
struct IsaRegister
{
	//UUID id = 0;				//generated id, track registers in app
	std::string name;			//string identifier e.g. r0, gpr0 - assy/disassy
	std::string friendlyName;	//human readable e.g. GP Data Register 0
	std::string description;	//write a novel if u want xo

	uint16_t bitWidth = 8;
	IsaRegisterType type = IsaRegisterType::Other;
	//std::string customTypeString; //todo is this necessary?

	bool readable = true;
	bool writable = true;

	IsaRegisterRole role = IsaRegisterRole::General;

	std::vector<IsaRegisterField> fields;

	std::vector<std::string> aliases;	//alternate names (e.g. SP is also r7)
	std::vector<std::string> tags;		//custom identifier tags

	//todo link to hardware? or link fields to hardware signal/bus?
};

/* Define register file within design */
struct IsaRegisterFile
{
	//UUID id = 0;
	std::string name;
	std::string friendlyName;
	std::string description;

	IsaRegisterFileType type;

	uint16_t defaultBitWidth = 8;

	std::map<std::string, IsaRegister> registers;
};