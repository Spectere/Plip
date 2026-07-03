// ValueType.h
//
// Represents an arbitrary value.

#pragma once

#include <cstdint>
#include <map>
#include <string>

struct ValueType {
    float ValueFloat {};
    double ValueDouble {};

    uint64_t ValueUInt {};
    int64_t ValueSInt {};

    std::string ValueString {};

    std::map<std::string, uint64_t> ValueRegs {};
};
