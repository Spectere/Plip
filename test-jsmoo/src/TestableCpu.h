// TestableCpu.h
//
// A CPU that is supported by this test framework.

#pragma once

#include <functional>
#include <string>

#include "IRunner.h"

struct TestableCpu {
    std::string Name;
    std::function<std::unique_ptr<IRunner>()> Create;
};
