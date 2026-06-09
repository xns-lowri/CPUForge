#pragma once

#include <string>
#include <vector>

#include "../../_types.h"
#include "../../project/Project.h"

#include "enumIsaDef.h"
#include "IsaContext.h"
#include "IsaAddress.h"

//instruction field description within encoding def - metadata
struct InstructionEncodingField {
	//UUID id = 0;
	std::string name;
	std::string description;

	uint16_t bitPosition;	//starting position in instruction
	uint16_t bitWidth;

	IsaInstructionFieldType fieldType = IsaInstructionFieldType::Other;
};

inline void to_json(json& j, const InstructionEncodingField& field) {
	j = json{
		{ "name", field.name },
		{ "description", field.description },
		{ "bitPosition", field.bitPosition },
		{ "bitWidth", field.bitWidth },
		{ "fieldType", ToString(field.fieldType) }
	};
}
inline void from_json(const json& j, InstructionEncodingField& field) {
	j.at("name").get_to(field.name);
	j.at("description").get_to(field.description);
	j.at("bitPosition").get_to(field.bitPosition);
	j.at("bitWidth").get_to(field.bitWidth);
	j.at("fieldType").get_to<IsaInstructionFieldType>(field.fieldType);
}

/* Encoding definitions - architecture data */
struct IsaInstructionEncodingFormat
{
	//UUID id = 0;
	std::string name;
	std::string description;

	uint16_t instructionWidth; // = 8;
	std::unordered_map<std::string, InstructionEncodingField> encodingFields;
};

inline void to_json(json& j, const IsaInstructionEncodingFormat& format) {
	j = json{
		{ "name", format.name },
		{ "description", format.description },
		{ "instructionWidth", format.instructionWidth },
		{ "encodingFields", format.encodingFields }
	};
}
inline void from_json(const json& j, IsaInstructionEncodingFormat& format) {
	j.at("name").get_to(format.name);
	j.at("description").get_to(format.description);
	j.at("instructionWidth").get_to(format.instructionWidth);
	j.at("encodingFields").get_to(format.encodingFields);
}
/* end metadata */


/* Instruction operand within ISA instruction definition */
struct IsaInstructionOperand
{
	//UUID id = 0;
	std::string name;
	std::string description;

	//UUID encodingFieldId; // = 0;	//points to instruction field definition
	std::string encodingField;

	//operand kind
	OperandRole role;
	OperandKind kind;

	std::optional<std::string> dataType;
	std::optional<std::string> addressingMode;
	//access flags

	std::vector<std::string> tags;
};

inline void to_json(json& j, const IsaInstructionOperand& operand) {
	j = json{
		{ "name", operand.name },
		{ "description", operand.description },
		{ "encodingField", operand.encodingField },
		{ "dataType", operand.dataType.value_or("none") },
		{ "addressingMode", operand.addressingMode.value_or("none") },
		{ "tags", operand.tags }
	};
}
inline void from_json(const json& j, IsaInstructionOperand& operand) {
	j.at("name").get_to(operand.name);
	j.at("description").get_to(operand.description);
	j.at("encodingField").get_to(operand.encodingField);

	if (j.at("dataType") == "none") { operand.dataType = std::nullopt; }
	else { j.at("dataType").get_to(operand.dataType); }

	if (j.at("addressingMode") == "none") { operand.addressingMode = std::nullopt; }
	else { j.at("addressingMode").get_to(operand.addressingMode); }

	j.at("tags").get_to(operand.tags);
}


/* Defines effects (state changes and context transitions) for instructions */
struct IsaInstructionEffects {
	std::string description;

	std::vector<IsaStateAccess> reads;
	std::vector<IsaStateAccess> writes;
	std::vector<IsaStateAccess> modifies;

	//todo conditions?
	//todo instruction logic?

	//context transitions
	std::vector<IsaContextTransition> transitions;

	std::optional<std::string> pseudoCode;
};

inline void to_json(json& j, const IsaInstructionEffects& effects) {
	j = json{
		{ "description", effects.description },
		{ "reads", effects.reads },
		{ "writes", effects.writes },
		{ "modifies", effects.modifies },
		{ "transitions", effects.transitions },
		{ "pseudoCode", effects.pseudoCode.value_or("none") }
	};
}
inline void from_json(const json& j, IsaInstructionEffects& effects) {
	j.at("description").get_to(effects.description);
	j.at("reads").get_to(effects.reads);
	j.at("writes").get_to(effects.writes);
	j.at("modifies").get_to(effects.modifies);
	j.at("transitions").get_to(effects.transitions);

	if (j.at("pseudoCode") == "none") { effects.pseudoCode = std::nullopt; }
	else { j.at("pseudoCode").get_to(effects.pseudoCode); }
}

//context rules structure for instruction
struct IsaInstructionContextRules
{
	//use OR logic for each rule
	//each rule has AND logic for 
	std::vector<IsaContextRule> allowedContexts;

	//isa features (extended/modified isas)
	std::vector<std::string> requiredFeatures;
	std::vector<std::string> forbiddenFeatures;

	//isa fault to throw on illegal/invalid instructions
	std::optional<std::string> faultIfInvalid;
};

inline void to_json(json& j, const IsaInstructionContextRules& contextRules) {
	j = json{
		{ "allowedContexts", contextRules.allowedContexts },
		{ "requiredFeatures", contextRules.requiredFeatures },
		{ "forbiddenFeatures", contextRules.forbiddenFeatures },
		{ "faultIfInvalid", contextRules.faultIfInvalid.value_or("none") }
	};
}
inline void from_json(const json& j, IsaInstructionContextRules& contextRules) {
	j.at("allowedContexts").get_to(contextRules.allowedContexts);
	j.at("requiredFeatures").get_to(contextRules.requiredFeatures);
	j.at("forbiddenFeatures").get_to(contextRules.forbiddenFeatures);

	if (j.at("faultIfInvalid") == "none") { contextRules.faultIfInvalid = std::nullopt; }
	else { j.at("faultIfInvalid").get_to(contextRules.faultIfInvalid); }
}


/* Struct for individual instructions within ISA */
struct IsaInstruction {
	//UUID id; // = 0;	//unique id for instruction - todo revise/remove?
	//UUID encodingId; // = 0;	//unique id
	std::string mnemonic;
	std::string name;
	std::string description;

	uint16_t opcode;	//opcode value assigned to instruction

	std::string encoding;	//link to relevant instruction encoding for format
	std::unordered_map<std::string, IsaInstructionOperand> operands; //operands	

	//semantics - todo vectors? requirement expression?
	IsaInstructionContextRules contextRules;	//context reqs for instruction
	IsaInstructionEffects effects;				//effects from instruction
	//todo execution model for instruction - in effects??

	std::vector<std::string> aliases;
	std::vector<std::string> tags;
};

inline void to_json(json& j, const IsaInstruction& instruction) {
	j = json{
		{ "mnemonic", instruction.mnemonic },
		{ "name", instruction.name },
		{ "description", instruction.description },

		{ "opcode", instruction.opcode },

		{ "encoding", instruction.encoding },
		{ "operands", instruction.operands },

		{ "contextRules", instruction.contextRules },
		{ "effects", instruction.effects },

		{ "aliases", instruction.aliases },
		{ "tags", instruction.tags }
	};
}
inline void from_json(const json& j, IsaInstruction& instruction) {
	j.at("mnemonic").get_to(instruction.mnemonic);
	j.at("name").get_to(instruction.name);
	j.at("description").get_to(instruction.description);

	j.at("opcode").get_to(instruction.opcode);

	j.at("encoding").get_to(instruction.encoding);
	j.at("operands").get_to(instruction.operands);

	j.at("contextRules").get_to(instruction.contextRules);
	j.at("effects").get_to(instruction.effects);

	j.at("aliases").get_to(instruction.aliases);
	j.at("tags").get_to(instruction.tags);
}