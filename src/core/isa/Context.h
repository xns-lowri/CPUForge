#pragma once
#include <string>
#include <vector>
#include <optional>

#include "../../_types.h"
#include "../../project/Project.h"

#include "enumIsaDef.h"

/* Represents arbitrary context dimensions in architecture */
struct IsaContextDimension
{
    UUID id = 0;
    std::string name;           // "privilege", "mode", "extension"
    std::string friendlyName;   // "Privilege Level", "CPU Mode", "ISA Extension"
    std::string description;

    std::vector<IsaContextValue> values;

    std::optional<std::string> registerFieldPath; // e.g. "status.privilege"
};

/* Represents possible states within each context dimension */
struct IsaContextValue
{
    UUID id = 0;
    std::string name;           // "user", "kernel", "interrupt", "debug"
    std::string friendlyName;   // "User", "Kernel", "Interrupt", "Debug"
    std::string description;

    std::optional<uint64_t> encodedValue;
};