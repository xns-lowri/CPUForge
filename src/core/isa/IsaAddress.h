#pragma once
#include <string>
#include <vector>
#include <optional>

#include "../../_types.h"
#include "../../project/Project.h"

#include "enumIsaDef.h"
#include "IsaContext.h"

//define translation between address spaces
struct IsaAddressTranslationStage {
	std::string name;
	std::string friendlyName;
	std::string description;

	std::string inputAddressSpace;
	std::string outputAddressSpace;

	IsaAddressTranslationKind addressTranslationKind;

	IsaContextRule activeRule;

	std::vector<std::string> possibleFaults;

	std::vector<std::string> tags;
};

struct IsaAddressingMode {
	std::string name;
	std::string friendlyName;
	std::string description;

	IsaAddressingModeKind kind;

	//operands?

	//expression effectiveAddressExpression??

	//address space id

	//possible faults

	std::vector<std::string> tags;
};