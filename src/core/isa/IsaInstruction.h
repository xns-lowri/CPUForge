#pragma once

#include <string>
#include <vector>

#include "../../_types.h"
#include "../../project/Project.h"

#include "enumIsaDef.h"

#include "IsaContext.h"

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

/* Encoding definitions - architecture data */
struct InstructionEncodingFormat
{
	//UUID id = 0;
	std::string name;
	std::string description;

	uint16_t instructionWidth; // = 8;
	std::unordered_map<std::string, InstructionEncodingField> encodingFields;
};

inline void to_json(json& j, const InstructionEncodingFormat& format) {
	j = json{
		{ "name", format.name },
		{ "description", format.description },
		{ "instructionWidth", format.instructionWidth },
		{ "encodingFields", format.encodingFields }
	};
}
/* end metadata */


/* Instruction operand within ISA instruction definition */
struct InstructionOperand
{
	//UUID id = 0;
	std::string name;
	std::string description;

	//UUID encodingFieldId; // = 0;	//points to instruction field definition
	std::string encodingFieldName;
};

inline void to_json(json& j, const InstructionOperand& operand) {
	j = json{
		{ "name", operand.name },
		{ "description", operand.description },
		{ "encodingFieldName", operand.encodingFieldName }
	};
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


/* Struct for individual instructions within ISA */
struct IsaInstruction {
	//UUID id; // = 0;	//unique id for instruction - todo revise/remove?
	//UUID encodingId; // = 0;	//unique id
	std::string mnemonic;
	std::string name;
	std::string description;

	uint16_t opcode;	//opcode value assigned to instruction


	std::string encodingName;	//link to relevant instruction encoding for format
	std::unordered_map<std::string, InstructionOperand> operands; //operands	

	//semantics - todo vectors? requirement expression?
	IsaInstructionContextRules contextRules;	//context reqs for instruction
	IsaInstructionEffects effects;				//effects from instruction

	std::vector<std::string> aliases;
	std::vector<std::string> tags;
};

inline void to_json(json& j, const IsaInstruction& instruction) {
	j = json{
		{ "mnemonic", instruction.mnemonic },
		{ "name", instruction.name },
		{ "description", instruction.description },
		{ "opcode", instruction.opcode },
		{ "encodingName", instruction.encodingName },
		{ "operands", instruction.operands },
		{ "contextRules", instruction.contextRules },
		{ "effects", instruction.effects },
		{ "aliases", instruction.aliases },
		{ "tags", instruction.tags }
	};
}