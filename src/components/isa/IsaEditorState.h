#pragma once
#include <vector>
#include "../../core/isa/IsaArchitecture.h"

struct IsaEditorState {
	std::vector<IsaDefinition> openDocuments;
};