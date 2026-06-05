#pragma once
#include <unordered_map>
#include "../../core/isa/IsaArchitecture.h"

struct IsaEditorState {
	std::unordered_map<UUID, IsaDefinition> openDocuments;
};