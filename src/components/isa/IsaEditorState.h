#pragma once

enum class IsaEditorTabState {
	Details,
	DataModel,
	Registers,
	MemoryIO,
	InstructionSet,
	ExecutionContext,
	Validation
};

struct IsaEditorState {
	//todo state for ViewIsaEditor window
	IsaEditorTabState tabState = IsaEditorTabState::Details;
};