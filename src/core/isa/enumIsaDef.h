#pragma once

enum class Endianness
{
	Little,
	Big,
	Custom,
	Other
};

//isa register types
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

enum class IsaStateObjectKind
{
	Register,
	RegisterField,
	Memory,

	Context,
	ProgramCounter,
	Stack,

	Custom
};