#pragma once
#include "../../_types.h"

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
	int DataModelSelectedType = -1;
	UUID DataModelSelectedTypeId = 0;
};