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
inline void from_json(const json& j, IsaRegisterType type) {
	type = IsaRegisterTypeFromString(j);
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
	Internal,
	Debug,
	Custom,
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
	case IsaRegisterRole::Internal:			return "Internal";
	case IsaRegisterRole::Debug:			return "Debug";
	case IsaRegisterRole::Custom:			return "Custom";
	case IsaRegisterRole::Other:			return "Other";
	default: return "Unknown";
	}
}
inline IsaRegisterRole IsaRegisterRoleFromString(const std::string& str) {
	if (str == "General")			return IsaRegisterRole::General;
	if (str == "ProgramCounter")	return IsaRegisterRole::ProgramCounter;
	if (str == "StackPointer")		return IsaRegisterRole::StackPointer;
	if (str == "FramePointer")		return IsaRegisterRole::FramePointer;
	if (str == "InterruptVector")	return IsaRegisterRole::InterruptVector;
	if (str == "MemoryMapping")		return IsaRegisterRole::MemoryMapping;
	if (str == "Status")			return IsaRegisterRole::Status;
	if (str == "Control")			return IsaRegisterRole::Control;
	if (str == "Special")			return IsaRegisterRole::Special;
	if (str == "Internal")			return IsaRegisterRole::Internal;
	if (str == "Debug")				return IsaRegisterRole::Debug;
	if (str == "Custom")			return IsaRegisterRole::Custom;
	if (str == "Other")				return IsaRegisterRole::Other;
	return IsaRegisterRole::Other; // default
}

inline void from_json(const json& j, IsaRegisterRole role) {
	role = IsaRegisterRoleFromString(j);
}


//Register file types - general categorisation
enum class IsaRegisterFileType
{
	GeneralPurpose,
	Address,

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
	case IsaRegisterFileType::Address:			return "Address";
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
inline IsaRegisterFileType IsaRegisterFileTypeFromString(const std::string& str) {
	if( str == "GeneralPurpose")	return IsaRegisterFileType::GeneralPurpose;
	if( str == "Address")			return IsaRegisterFileType::Address;
	if( str == "FloatingPoint" )	return IsaRegisterFileType::FloatingPoint;
	if( str == "Vector")			return IsaRegisterFileType::Vector;
	if( str == "Status")			return IsaRegisterFileType::Status;
	if( str == "Control")			return IsaRegisterFileType::Control;
	if( str == "Special")			return IsaRegisterFileType::Special;
	if( str == "Custom")			return IsaRegisterFileType::Custom;
	if( str == "Internal")			return IsaRegisterFileType::Internal;
	if( str == "Debug" )			return IsaRegisterFileType::Debug;
	if( str == "Other" )			return IsaRegisterFileType::Other;
	return IsaRegisterFileType::Other;
}
inline void from_json(const json& j, IsaRegisterFileType& type) {
	type = IsaRegisterFileTypeFromString(j);
}

//instruction field types
enum class IsaInstructionFieldType
{
	Opcode,
	//OpcodeAuto,	//for grouped functions?

	Register,
	Immediate,
	RegisterAddress,
	ImmediateAddress,

	//offset and other memory modes?

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
inline IsaInstructionFieldType IsaInstructionFieldTypeFromString(const std::string& str) {
	if (str == "Opcode")				return IsaInstructionFieldType::Opcode;
	if (str == "Register")				return IsaInstructionFieldType::Register;
	if( str == "Immediate" )			return IsaInstructionFieldType::Immediate;
	if (str == "RegisterAddress")		return IsaInstructionFieldType::RegisterAddress;
	if (str == "ImmediateAddress")		return IsaInstructionFieldType::ImmediateAddress;
	if (str == "Flag")					return IsaInstructionFieldType::Flag;
	if( str == "Label" )				return IsaInstructionFieldType::Label;
	if (str == "Special")				return IsaInstructionFieldType::Special;
	if (str == "Custom")				return IsaInstructionFieldType::Custom;
	if (str == "Internal")				return IsaInstructionFieldType::Internal;
	if( str == "Debug" )				return IsaInstructionFieldType::Debug;
	if( str == "Other" )				return IsaInstructionFieldType::Other;
	return IsaInstructionFieldType::Other;
}
inline void from_json(const json& j, IsaInstructionFieldType& type) {
	type = IsaInstructionFieldTypeFromString(j);
}

//context for instruction effects
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
inline IsaStateObjectKind IsaStateObjectKindFromString(const std::string& str) {
	if (str == "Register")			return IsaStateObjectKind::Register;
	if (str == "RegisterField")		return IsaStateObjectKind::RegisterField;
	if (str == "Memory")			return IsaStateObjectKind::Memory;
	if (str == "Context")			return IsaStateObjectKind::Context;
	if (str == "ProgramCounter")	return IsaStateObjectKind::ProgramCounter;
	if (str == "Stack")				return IsaStateObjectKind::Stack;
	if (str == "Custom")			return IsaStateObjectKind::Custom;
	if (str == "Other")				return IsaStateObjectKind::Other;
	return IsaStateObjectKind::Other;
}
inline void from_json(const json& j, IsaStateObjectKind& kind) {
	kind = IsaStateObjectKindFromString(j);
}

enum class IsaDataTypeKind {
	Integer,
	Float,
	Bool,
	Pointer,
	Vector,
	Packed,
	Custom,
	Other
};
inline std::string ToString(IsaDataTypeKind kind) {
	switch (kind) {
	case IsaDataTypeKind::Integer:	return "Integer";
	case IsaDataTypeKind::Float:	return "Float";
	case IsaDataTypeKind::Bool:	return "Bool";
	case IsaDataTypeKind::Pointer:	return "Pointer";
	case IsaDataTypeKind::Vector:	return "Vector";
	case IsaDataTypeKind::Packed:	return "Packed";
	case IsaDataTypeKind::Custom:	return "Custom";
	case IsaDataTypeKind::Other:	return "Other";
	default:					return "Other";
	}
}
inline IsaDataTypeKind DataTypeKindFromString(std::string str) {
	if (str == "Integer")	return IsaDataTypeKind::Integer;
	if (str == "Float")		return IsaDataTypeKind::Float;
	if (str == "Bool")		return IsaDataTypeKind::Bool;
	if (str == "Pointer")	return IsaDataTypeKind::Pointer;
	if (str == "Vector")	return IsaDataTypeKind::Vector;
	if (str == "Packed")	return IsaDataTypeKind::Packed;
	if (str == "Custom")	return IsaDataTypeKind::Custom;
	if (str == "Other")		return IsaDataTypeKind::Other;
	return IsaDataTypeKind::Other;
}
inline void from_json(const json& j, IsaDataTypeKind& kind) {
	kind = DataTypeKindFromString(j);
}

//todo implement:
//addressing strategy for memory space
enum class IsaDataUnit
{
	Bit,
	Byte,
	Word,
	Custom,
	Other
};

inline std::string ToString(const IsaDataUnit& unit) {
	switch (unit) {
	case (IsaDataUnit::Bit):	return "Bit";
	case (IsaDataUnit::Byte):	return "Byte";
	case (IsaDataUnit::Word):	return "Word";
	case (IsaDataUnit::Custom): return "Custom";
	default:					return "Other";
	}
}
inline IsaDataUnit AddressUnitFromString(const std::string& str) {
	if (str == "Bit")		return IsaDataUnit::Bit;
	if (str == "Byte")		return IsaDataUnit::Byte;
	if (str == "Word")		return IsaDataUnit::Word;
	if (str == "Custom")	return IsaDataUnit::Custom;
	return IsaDataUnit::Other;
}
inline void from_json(const json& j, IsaDataUnit& unit) {
	unit = AddressUnitFromString(j);
}

//consistency model for architecture
enum class IsaMemoryConsistencyModel {
	Sequential,
	Weak,
	Custom
};

inline std::string ToString(const IsaMemoryConsistencyModel& model) {
	switch (model) {
	case IsaMemoryConsistencyModel::Sequential:	return "Sequential";
	case IsaMemoryConsistencyModel::Weak:			return "Weak";
	default:							return "Custom";
	}
}
inline IsaMemoryConsistencyModel MemoryConsistencyModelFromString(const std::string& str) {
	if (str == "Sequential")	return IsaMemoryConsistencyModel::Sequential;
	if (str == "Weak")			return IsaMemoryConsistencyModel::Weak;
	return IsaMemoryConsistencyModel::Custom;

}
inline void from_json(const json& j, IsaMemoryConsistencyModel& model) {
	model = MemoryConsistencyModelFromString(j);
}

enum class IsaMemoryAccessKind {
	Read,
	Write,
	ReadModifyWrite,
	ExecuteFetch,
	Prefetch,
	Custom
};
enum class IsaAddressTranslationKind {
	None,
	Banked,
	Segmented,
	Paged,
	MemoryMapped,
	Custom
};
enum class IsaAddressingModeKind {
	Implied,
	Immediate,
	Register,
	RegisterIndirect,
	Direct,
	Absolute,
	BaseOffset,
	Indexed,
	BaseIndexScaleOffset,
	PcRelative,
	StackRelative,
	MemoryIndirect,
	PortDirect,
	PortIndirect,
	Custom
};
enum class OperandRole {
	Source,
	Destination,
	SourceDestination,
	Address,
	Immediate,
	Condition,
	Offset,
	Target,
	Custom
};
enum class OperandKind {
	Register,
	RegisterField,
	Immediate,
	Memory,
	Address,
	Port,
	Label,
	Flag,
	ContextState,
	Custom
};
// ***** TODO JSON HANDLERS FOR ABOVE *****

/* Custom enum definitions for register field values */
struct IsaEnumValue
{
	//UUID id = 0;
	std::string name;
	std::string friendlyName;
	std::string description;
	uint64_t value;
};

inline void to_json(json& j, const IsaEnumValue& enumVal) {
	j = json{
		{ "name", enumVal.name },
		{ "friendlyName", enumVal.friendlyName },
		{ "value", enumVal.value },
		{ "description", enumVal.description }
	};
}
inline void from_json(const json& j, IsaEnumValue& enumVal) {
	j.at("name").get_to(enumVal.name);
	j.at("friendlyName").get_to(enumVal.friendlyName);
	j.at("value").get_to(enumVal.value);
	j.at("description").get_to(enumVal.description);
}
