#pragma once

#include <string>
#include <vector>

//#include "../project/Project.h"

//instruction field description within encoding def - metadata
struct InstructionEncodingField {
	ItemHeader header;

	uint32_t bitPosition;	//starting position in instruction
	uint32_t bitWidth;

	FieldType fieldType = FieldType::Other;
};

//encoding definitions - metadata
struct InstructionEncoding
{
	ItemHeader header;

	uint32_t instructionWidth; // = 8;
	std::unordered_map<UUID, InstructionEncodingField> encodingFields;
};
/* end metadata */

/* Instruction operand within ISA instruction definition */
struct InstructionOperand
{
	ItemHeader header;

	UUID encodingFieldId; // = 0;	//points to instruction field definition
};

/* Struct for individual instructions within ISA */
struct InstructionDef {
	UUID instructionId; // = 0;	//unique id for instruction
	std::string mnemonic;
	std::string name;

	std::string description;

	uint32_t opcode; // = 0;	//opcode value assigned to instruction
	
	UUID encodingId; // = 0;	//unique id

	std::unordered_map<UUID, InstructionOperand> operands; //operands
	//semantics todo

	std::vector<std::string> tags;
};
