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

/* Structs for Instruction context rules definition */
struct IsaContextRequirement
{
    //individual requirement within context rule
    std::string name;
    std::string dimensionId;              // "privilege"
    //list of allowed states within context dimension, use OR logic
    std::vector<IsaContextState> allowedStates; // ["kernel", "machine"]
};

struct IsaContextRule 
{
    std::string name;
    std::vector<IsaContextRequirement> requiredStates;
    //vector may contain required states across dimensions
    
    //context requirement defines allowed states per dimension
    //context rule defines AND logic for requirements list
    //vector of rules then ORed in instruction
};

/* Defines state accesses (read/write/modify) for inst. effects */
struct IsaStateAccess
{
    IsaStateObjectKind kind;
    std::string target; // "r0", "status.z", "status.privilege", "memory", "pc"
    std::optional<std::string> condition;
};

/* Possible context transitions for instruction effects */
struct IsaContextTransition {
    std::string name;
    std::string dimensionId;

    std::vector<IsaContextState> fromStates;
    IsaContextState toState;
    //todo condition?
};