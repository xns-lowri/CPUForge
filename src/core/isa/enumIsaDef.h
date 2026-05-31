#pragma once

enum class Endianness
{
	Little,
	Big,
	Custom,
	Other
};

//isa register types
enum class RegisterType
{
	Data,
	Address,
	Control,
	Flag,
	Custom,
	Other
};

//instruction field types
enum class FieldType
{
	Opcode,
	Register,
	Immediate,
	Address,
	Flag,
	Label,
	Custom,
	Other
};