#pragma once

enum class Endianness
{
	Little,
	Big,
	Custom,
	Other
};

inline std::string ToString(Endianness endianness) {
	switch (endianness) {
	case Endianness::Little:	return "Little";
	case Endianness::Big:		return "Big";
	case Endianness::Custom:	return "Custom";
	case Endianness::Other:		return "Other";
	default:					return "Unknown";
	}
}

inline Endianness EndiannessFromString(const std::string& str) {
	if (str == "Little")	return Endianness::Little;
	if (str == "Big")		return Endianness::Big;
	if (str == "Custom")	return Endianness::Custom;
	if (str == "Other")		return Endianness::Other;
	return Endianness::Other; // default
};

inline void from_json(const json& j, Endianness& endianness) {
	endianness = EndiannessFromString(j);
};


/* ISA register types */
enum class IsaRegisterType
{
	Data,
	Address,
	Status,
	Control,
	//Flag,
	Custom,
	Other
};

inline std::string ToString(IsaRegisterType type) {
	switch (type) {
	case IsaRegisterType::Data:		return "Data";
	case IsaRegisterType::Address:	return "Address";
	case IsaRegisterType::Status:	return "Status";
	case IsaRegisterType::Control:	return "Control";
	case IsaRegisterType::Custom:	return "Custom";
	case IsaRegisterType::Other:	return "Other";
	default: return "Unknown";
	}
}

inline IsaRegisterType IsaRegisterTypeFromString(const std::string& str) {
	if (str == "Data")		return IsaRegisterType::Data;
	if (str == "Address")	return IsaRegisterType::Address;
	if (str == "Status")	return IsaRegisterType::Status;
	if (str == "Control")	return IsaRegisterType::Control;
	if (str == "Custom")	return IsaRegisterType::Custom;
	if (str == "Other")		return IsaRegisterType::Other;
	return IsaRegisterType::Other; // default
}

//Register roles - todo revise?
enum class IsaRegisterRole
{
	General,

	ProgramCounter,
	StackPointer,
	FramePointer,

	InterruptVector,
	MemoryMapping,

	Status,
	Control,

	Special,
	Custom,
	Internal,
	Debug,
	Other
};

inline std::string ToString(IsaRegisterRole role) {
	switch (role) {
	case IsaRegisterRole::General:			return "General";
	case IsaRegisterRole::ProgramCounter:	return "ProgramCounter";
	case IsaRegisterRole::StackPointer:		return "StackPointer";
	case IsaRegisterRole::FramePointer:		return "FramePointer";
	case IsaRegisterRole::InterruptVector:	return "InterruptVector";
	case IsaRegisterRole::MemoryMapping:	return "MemoryMapping";
	case IsaRegisterRole::Status:			return "Status";
	case IsaRegisterRole::Control:			return "Control";
	case IsaRegisterRole::Special:			return "Special";
	case IsaRegisterRole::Custom:			return "Custom";
	case IsaRegisterRole::Internal:			return "Internal";
	case IsaRegisterRole::Debug:			return "Debug";
	case IsaRegisterRole::Other:			return "Other";
	default: return "Unknown";
	}
}


//Register file types
enum class IsaRegisterFileType
{
	GeneralPurpose,
	FloatingPoint,
	Vector,

	Status,
	Control,

	Special,
	Custom,
	Internal,
	Debug,
	Other
};

inline std::string ToString(IsaRegisterFileType type) {
	switch (type) {
	case IsaRegisterFileType::GeneralPurpose:	return "GeneralPurpose";
	case IsaRegisterFileType::FloatingPoint:	return "FloatingPoint";
	case IsaRegisterFileType::Vector:			return "Vector";
	case IsaRegisterFileType::Status:			return "Status";
	case IsaRegisterFileType::Control:			return "Control";
	case IsaRegisterFileType::Special:			return "Special";
	case IsaRegisterFileType::Custom:			return "Custom";
	case IsaRegisterFileType::Internal:			return "Internal";
	case IsaRegisterFileType::Debug:			return "Debug";
	case IsaRegisterFileType::Other:			return "Other";
	default: return "Unknown";
	}
}

//instruction field types
enum class IsaInstructionFieldType
{
	Opcode,
	Register,
	Immediate,
	RegisterAddress,
	ImmediateAddress,

	Flag,
	Label,

	Special,
	Custom,
	Internal,
	Debug,
	Other
};

inline std::string ToString(IsaInstructionFieldType type) {
	switch (type) {
	case IsaInstructionFieldType::Opcode:			return "Opcode";
	case IsaInstructionFieldType::Register:			return "Register";
	case IsaInstructionFieldType::Immediate:		return "Immediate";
	case IsaInstructionFieldType::RegisterAddress:	return "RegisterAddress";
	case IsaInstructionFieldType::ImmediateAddress:	return "ImmediateAddress";
	case IsaInstructionFieldType::Flag:				return "Flag";
	case IsaInstructionFieldType::Label:			return "Label";
	case IsaInstructionFieldType::Special:			return "Special";
	case IsaInstructionFieldType::Custom:			return "Custom";
	case IsaInstructionFieldType::Internal:			return "Internal";
	case IsaInstructionFieldType::Debug:			return "Debug";
	case IsaInstructionFieldType::Other:			return "Other";
	default: return "Unknown";
	}
}

enum class IsaStateObjectKind
{
	Register,
	RegisterField,
	Memory,

	Context,
	ProgramCounter,
	Stack,

	Custom,
	Other
};

inline std::string ToString(IsaStateObjectKind kind) {
	switch (kind) {
	case IsaStateObjectKind::Register:		return "Register";
	case IsaStateObjectKind::RegisterField:	return "RegisterField";
	case IsaStateObjectKind::Memory:		return "Memory";
	case IsaStateObjectKind::Context:		return "Context";
	case IsaStateObjectKind::ProgramCounter:return "ProgramCounter";
	case IsaStateObjectKind::Stack:			return "Stack";
	case IsaStateObjectKind::Custom:		return "Custom";
	case IsaStateObjectKind::Other:			return "Other";
	default: return "Unknown";
	}
}