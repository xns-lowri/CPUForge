#pragma once
#include <unordered_map>
#include "../../core/isa/IsaDefinition.h"

struct IsaDocumentStore {
	std::unordered_map<UUID, IsaDefinition> openDocuments;
};