#pragma once

#include <string>
#include <vector>

#include "../../_types.h"
#include "../../project/Project.h"

#include "enumIsaDef.h"

#include "Context.h"

//instruction field description within encoding def - metadata
struct InstructionEncodingField {
	//UUID id = 0;
	std::string name;
	std::string description;

	uint16_t bitPosition;	//starting position in instruction
	uint16_t bitWidth;

	IsaInstructionFieldType fieldType = IsaInstructionFieldType::Other;
};

/* Encoding definitions - architecture data */
struct InstructionEncodingFormat
{
	//UUID id = 0;
	std::string name;
	std::string description;

	uint16_t instructionWidth; // = 8;
	std::unordered_map<std::string, InstructionEncodingField> encodingFields;
};
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