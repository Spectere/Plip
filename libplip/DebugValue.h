// DebugValue.h
//
// TODO: Insert description here.

#pragma once

#include <cstdint>
#include <string>
#include <utility>

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
        String,
        Flag
    };

    struct DebugValue {
        DebugValue() : Type(DebugValueType::Unknown) {}
        explicit DebugValue(std::string value) : Type(DebugValueType::String), ValueString(std::move(value)) {}
        explicit DebugValue(const DebugValueType type, const uint64_t value) : Type(type), ValueInt(value) {}
        explicit DebugValue(const DebugValueType type, const double value) : Type(type), ValueFloat(value) {}
        explicit DebugValue(const bool value) : Type(DebugValueType::Flag), ValueFlag(value) {}

        DebugValueType Type;

        union {
            uint64_t ValueInt {};
            double ValueFloat;
        };

        std::string ValueString {};
        bool ValueFlag {};
    };
}
