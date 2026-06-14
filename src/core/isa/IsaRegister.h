#pragma once
#include <string>
#include <vector>

#include "../../_types.h"
#include "../../project/Project.h"

#include "enumIsaDef.h"

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

inline void to_json(json& j, const IsaRegisterField& field) {
	j = json{
		{ "name", field.name },
		{ "friendlyName", field.friendlyName },
		{ "description", field.description },

		{ "startOffset", field.startOffset },
		{ "bitWidth", field.bitWidth },
		{ "readable", field.readable },
		{ "writable", field.writable },
		{ "resetValue", field.resetValue.value_or(-1) },
		{ "enumValues", field.enumValues }
	};
}
inline void from_json(const json& j, IsaRegisterField& field) {
	j.at("name").get_to(field.name);
	j.at("friendlyName").get_to(field.friendlyName);
	j.at("description").get_to(field.description);

	j.at("startOffset").get_to(field.startOffset);
	j.at("bitWidth").get_to(field.bitWidth);
	j.at("readable").get_to(field.readable);
	j.at("writable").get_to(field.writable);

	if (j.at("resetValue") == -1) { field.resetValue = std::nullopt; }
	else { j.at("resetValue").get_to(field.resetValue); }

	j.at("enumValues").get_to(field.enumValues);
}

/* Define single register within design */
struct IsaRegister
{
	UUID id = 0;				//generated id, track registers in app
	std::string name = "";			//string identifier e.g. r0, gpr0 - assy/disassy
	std::string friendlyName = "";	//human readable e.g. GP Data Register 0
	std::string description = "";	//write a novel if u want xo

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

inline void to_json(json& j, const IsaRegister& reg) {
	j = json{
		{ "name", reg.name },
		{ "friendlyName", reg.friendlyName },
		{ "description", reg.description },

		{ "bitWidth", reg.bitWidth },
		{ "type", ToString(reg.type) },
		{ "readable", reg.readable },
		{ "writable", reg.writable },
		{ "role", ToString(reg.role) },
		{ "fields", reg.fields },
		{ "aliases", reg.aliases },
		{ "tags", reg.tags }
	};
}
inline void from_json(const json& j, IsaRegister& reg) {
	j.at("name").get_to(reg.name);
	j.at("friendlyName").get_to(reg.friendlyName);
	j.at("description").get_to(reg.description);

	j.at("bitWidth").get_to(reg.bitWidth);
	j.at("type").get_to<IsaRegisterType>(reg.type);
	j.at("readable").get_to(reg.readable);
	j.at("writable").get_to(reg.writable);
	j.at("role").get_to<IsaRegisterRole>(reg.role);
	j.at("fields").get_to(reg.fields);
	j.at("aliases").get_to(reg.aliases);
	j.at("tags").get_to(reg.tags);
}

/* Define register file within design */
struct IsaRegisterFile
{
	UUID id = 0;
	std::string name = "";
	std::string friendlyName = "";
	std::string description = "";

	IsaRegisterFileType type = IsaRegisterFileType::GeneralPurpose;

	uint16_t defaultBitWidth = 8;

	std::map<UUID, IsaRegister> registers;
};

inline void to_json(json& j, const IsaRegisterFile& regFile) {
	j = json{
		{ "id", regFile.id },

		{ "name", regFile.name },
		{ "friendlyName", regFile.friendlyName },
		{ "description", regFile.description },

		{ "type", ToString(regFile.type) },
		{ "defaultBitWidth", regFile.defaultBitWidth },
		{ "registers", regFile.registers }
	};
}
inline void from_json(const json& j, IsaRegisterFile& regFile) {
	j.at("id").get_to(regFile.id);

	j.at("name").get_to(regFile.name);
	j.at("friendlyName").get_to(regFile.friendlyName);
	j.at("description").get_to(regFile.description);

	j.at("type").get_to<IsaRegisterFileType>(regFile.type);
	j.at("defaultBitWidth").get_to(regFile.defaultBitWidth);
	j.at("registers").get_to(regFile.registers);
}