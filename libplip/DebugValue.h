// DebugValue.h
//
// TODO: Insert description here.

#pragma once

#include <cstdint>
#include <string>

namespace Plip {
    enum class DebugValueType {
        Unknown,
        Int8,
        Int16Le,
        Int16Be,
        Int32Le,
        Int32Be,
        Int64Le,
        Int64Be,
        Float32Le,
        Float32Be,
        Float64Le,
        Float64Be,
        String
    };

    struct DebugValue {
        DebugValue() : Type(DebugValueType::Unknown) {}
        explicit DebugValue(const std::string& value) : Type(DebugValueType::String), ValueString(value) {}
        DebugValue(const DebugValueType type, const uint64_t value) : Type(type), ValueInt(value) {}
        DebugValue(const DebugValueType type, const double value) : Type(type), ValueFloat(value) {}

        DebugValueType Type;

        union {
            uint64_t ValueInt {};
            double ValueFloat;
        };

        std::string ValueString {};
    };
}
