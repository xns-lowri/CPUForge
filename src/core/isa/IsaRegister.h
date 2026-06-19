#pragma once
#include <string>
#include <vector>

#include "../../_types.h"
#include "../../project/Project.h"

#include "enumIsaDef.h"
#include "IsaContext.h"

/* Defines individual fields within register 
* - gp registers may have 1 field: r0(16) has data(16) - or just access register direct
* - special registers may have more: FLAGS has PRV(2), Z/N/C/V(1), etc
*/
struct IsaRegisterField {
	UUID id = 0;			
	std::string name;			
	std::string friendlyName;		
	std::string description;

	uint16_t startOffset = 0;
	uint16_t bitWidth = 8;

	bool allowRead = true;
	bool allowWrite = true;

	std::vector<IsaContextRule> allowedReadContexts;
	std::vector<IsaContextRule> allowedWriteContexts;

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

		{ "allowRead", field.allowRead },
		{ "allowWrite", field.allowWrite },
		{ "allowedReadContexts", field.allowedReadContexts },
		{ "allowedWriteContexts", field.allowedWriteContexts },

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

	j.at("allowRead").get_to(field.allowRead);
	j.at("allowWrite").get_to(field.allowWrite);
	j.at("allowedReadContexts").get_to(field.allowedReadContexts);
	j.at("allowedWriteContexts").get_to(field.allowedWriteContexts);

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

	bool allowRead = true;
	bool allowWrite = true;

	std::vector<IsaContextRule> allowedReadContexts;
	std::vector<IsaContextRule> allowedWriteContexts;

	IsaRegisterRole role = IsaRegisterRole::General;

	std::map<UUID, IsaRegisterField> fields;

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

		{ "allowRead", reg.allowRead },
		{ "allowWrite", reg.allowWrite },
		{ "allowedReadContexts", reg.allowedReadContexts },
		{ "allowedWriteContexts", reg.allowedWriteContexts },

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

	j.at("allowRead").get_to(reg.allowRead);
	j.at("allowWrite").get_to(reg.allowWrite);
	j.at("allowedReadContexts").get_to(reg.allowedReadContexts);
	j.at("allowedWriteContexts").get_to(reg.allowedWriteContexts);

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

	bool allowRead = true;
	bool allowWrite = true;

	std::vector<IsaContextRule> allowedReadContexts;
	std::vector<IsaContextRule> allowedWriteContexts;

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

		{ "allowRead", regFile.allowRead },
		{ "allowWrite", regFile.allowWrite },
		{ "allowedReadContexts", regFile.allowedReadContexts },
		{ "allowedWriteContexts", regFile.allowedWriteContexts },

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

	j.at("allowRead").get_to(regFile.allowRead);
	j.at("allowWrite").get_to(regFile.allowWrite);
	j.at("allowedReadContexts").get_to(regFile.allowedReadContexts);
	j.at("allowedWriteContexts").get_to(regFile.allowedWriteContexts);

	j.at("type").get_to<IsaRegisterFileType>(regFile.type);
	j.at("defaultBitWidth").get_to(regFile.defaultBitWidth);
	j.at("registers").get_to(regFile.registers);
}