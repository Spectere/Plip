// ExpectedResults.h
//
// The expected results of a unit test.

#pragma once

#include "ResultType.h"
#include "ValueType.h"

struct Results {
    ResultType Type = ResultType::None;
    ValueType Value {};
};
