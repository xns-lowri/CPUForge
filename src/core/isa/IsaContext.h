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

	std::string contextDimension;
    std::optional<uint16_t> encodedValue;   //hardware register value
};

inline void to_json(json& j, const IsaContextState& state) {
	j = json{
		{ "name", state.name },
		{ "friendlyName", state.friendlyName },
		{ "description", state.description },

		{ "contextDimension", state.contextDimension },
		{ "encodedValue", state.encodedValue.value_or(-1) }
	};
}
inline void from_json(const json& j, IsaContextState& state) {
	j.at("name").get_to(state.name);
	j.at("friendlyName").get_to(state.friendlyName);
	j.at("description").get_to(state.description);

	j.at("contextDimension").get_to(state.contextDimension);

	if (j.at("encodedValue") == -1) { state.encodedValue = std::nullopt; }
	else { j.at("encodedValue").get_to(state.encodedValue); }
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
inline void from_json(const json& j, IsaContextRequirement& requirement) {
	j.at("name").get_to(requirement.name);
	j.at("dimensionId").get_to(requirement.dimensionId);
	j.at("allowedStates").get_to(requirement.allowedStates);
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
inline void from_json(const json& j, IsaContextRule& rule) {
	j.at("name").get_to(rule.name);
	j.at("requiredStates").get_to(rule.requiredStates);
}

/* Context Dimensions - abstract context state handling */
struct IsaContextDimension
{
    //UUID id = 0;
    std::string name;           // "privilege", "mode", "extension"
    std::string friendlyName;   // "Privilege Level", "CPU Mode", "ISA Extension"
    std::string description;

	//todo add enum 'context kind'?? adds specificity, useful context?
	//or later add custom user-enum handler??

    std::vector<IsaContextState> possibleStates; //all possible states within dimension

    std::optional<std::string> registerFieldPath; // e.g. "status.privilege"
};

inline void to_json(json& j, const IsaContextDimension& dimension) {
	j = json{
		{ "name", dimension.name },
		{ "friendlyName", dimension.friendlyName },
		{ "description", dimension.description },
		{ "possibleStates", dimension.possibleStates },
		{ "registerFieldPath", dimension.registerFieldPath.value_or("null") }
	};
}
inline void from_json(const json& j, IsaContextDimension& dimension) {
	j.at("name").get_to(dimension.name);
	j.at("friendlyName").get_to(dimension.friendlyName);
	j.at("description").get_to(dimension.description);

	j.at("possibleStates").get_to(dimension.possibleStates);

	if (j.at("registerFieldPath") == "null") { dimension.registerFieldPath = std::nullopt; }
	else { j.at("registerFieldPath").get_to(dimension.registerFieldPath); }
}

/* Defines state accesses (read/write/modify) for instruction effects */
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
inline void from_json(const json& j, IsaStateAccess& access) {
	j.at("kind").get_to<IsaStateObjectKind>(access.kind);
	j.at("target").get_to(access.target);

	if (j.at("condition") == "none") { access.condition = std::nullopt; }
	else { j.at("condition").get_to(access.condition); }
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
inline void from_json(const json& j, IsaContextTransition& trans) {
	j.at("name").get_to(trans.name);
	j.at("dimensionId").get_to(trans.dimensionId);
	j.at("fromStates").get_to(trans.fromStates);
	j.at("toState").get_to(trans.toState);
}