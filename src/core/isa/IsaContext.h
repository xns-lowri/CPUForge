#pragma once
#include <string>
#include <vector>
#include <optional>

#include "../../_types.h"
#include "../../project/Project.h"

#include "enumIsaDef.h"


/* Represents possible states within each context dimension */
struct IsaContextState
{
    //UUID id = 0;
    std::string name;           // "user", "kernel", "interrupt", "debug"
    std::string friendlyName;   // "User", "Kernel", "Interrupt", "Debug"
    std::string description;

    std::optional<uint16_t> encodedValue;   //hardware register value
};

inline void to_json(json& j, const IsaContextState& state) {
	j = json{
		{ "name", state.name },
		{ "friendlyName", state.friendlyName },
		{ "description", state.description },
		{ "encodedValue", state.encodedValue.value_or(0) }
	};
}

/* Context Dimensions - abstract context state handling */
struct IsaContextDimension
{
    //UUID id = 0;
    std::string name;           // "privilege", "mode", "extension"
    std::string friendlyName;   // "Privilege Level", "CPU Mode", "ISA Extension"
    std::string description;

    std::vector<IsaContextState> states; //all possible states within dimension

    std::optional<std::string> registerFieldPath; // e.g. "status.privilege"
};

inline void to_json(json& j, const IsaContextDimension& dimension) {
	j = json{
		{ "name", dimension.name },
		{ "friendlyName", dimension.friendlyName },
		{ "description", dimension.description },
		{ "states", dimension.states },
		{ "registerFieldPath", dimension.registerFieldPath.value_or("none") }
	};
}

/* Structs for Instruction context rules definition */
struct IsaContextRequirement
{
    //individual requirement within context rule
    std::string name;
    std::string dimensionId;              // "privilege"
    //list of allowed states within context dimension, use OR logic
    std::vector<IsaContextState> allowedStates; // ["kernel", "machine"]
};

inline void to_json(json& j, const IsaContextRequirement& requirement) {
	j = json{
		{ "name", requirement.name },
		{ "dimensionId", requirement.dimensionId },
		{ "allowedStates", requirement.allowedStates }
	};
}

struct IsaContextRule 
{
    std::string name;
    std::vector<IsaContextRequirement> requiredStates;
    //vector may contain required states across dimensions
    
    //context requirement defines allowed states per dimension
    //context rule defines AND logic for requirements list
    //vector of rules then ORed in instruction
};

inline void to_json(json& j, const IsaContextRule& rule) {
	j = json{
		{ "name", rule.name },
		{ "requiredStates", rule.requiredStates }
	};
}

/* Defines state accesses (read/write/modify) for inst. effects */
struct IsaStateAccess
{
    IsaStateObjectKind kind;
    std::string target; // "r0", "status.z", "status.privilege", "memory", "pc"
    std::optional<std::string> condition;
};

inline void to_json(json& j, const IsaStateAccess& access) {
	j = json{
		{ "kind", ToString(access.kind) },
		{ "target", access.target },
		{ "condition", access.condition.value_or("none") }
	};
}

/* Possible context transitions for instruction effects */
struct IsaContextTransition {
    std::string name;
    std::string dimensionId;

    std::vector<std::string> fromStates;
    std::string toState;
    //todo condition?
};

inline void to_json(json& j, const IsaContextTransition& transition) {
	j = json{
		{ "name", transition.name },
		{ "dimensionId", transition.dimensionId },
		{ "fromStates", transition.fromStates },
		{ "toState", transition.toState }
	};
}