#pragma once
#include <string>
#include <vector>
#include <optional>
#include "../_types.h"

#include "enumIsaDef.h"

struct IsaContextDimension
{
    UUID id = 0;
    std::string tag;          // "privilege", "mode", "extension"
    std::string name;        // "Privilege Level", "CPU Mode", "ISA Extension"

    std::vector<IsaContextValue> values;
};

struct IsaContextValue
{
    UUID id = 0;
    std::string id;          // "user", "kernel", "interrupt", "debug"
    std::string name;        // "User", "Kernel", "Interrupt", "Debug"

    std::optional<uint64_t> encodedValue;
    std::string description;
};