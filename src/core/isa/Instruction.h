#pragma once

#include <string>
#include <vector>

#include "../../_types.h"
#include "../../project/Project.h"

#include "enumIsaDef.h"

//instruction field description within encoding def - metadata
struct InstructionEncodingField {
	UUID id = 0;
	std::string name;
	std::string description;

	uint16_t bitPosition;	//starting position in instruction
	uint16_t bitWidth;

	FieldType fieldType = FieldType::Other;
};

//encoding definitions - metadata
struct InstructionEncoding
{
	UUID id = 0;
	std::string name;
	std::string description;

	uint16_t instructionWidth; // = 8;
	std::unordered_map<UUID, InstructionEncodingField> encodingFields;
};
/* end metadata */


/* Instruction operand within ISA instruction definition */
struct InstructionOperand
{
	UUID id = 0;
	std::string name;
	std::string description;

	UUID encodingFieldId; // = 0;	//points to instruction field definition
};

/* Struct for individual instructions within ISA */
struct InstructionDef {
	UUID id; // = 0;	//unique id for instruction
	std::string mnemonic;
	std::string name;

	std::string description;

	uint16_t opcode; // = 0;	//opcode value assigned to instruction

	UUID encodingId; // = 0;	//unique id

	std::unordered_map<UUID, InstructionOperand> operands; //operands
	//semantics todo

	std::vector<std::string> tags;
};