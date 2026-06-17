include(CTest)
include(Catch)

add_executable(${test_name}
    # Common
    Cpu/MockCpu.h

    # CHIP-8
    Cpu/Chip8/Chip8Ops.cpp

    # MOS 6502
    Cpu/Mos6502/Mos6502Common.h
    Cpu/Mos6502/Mos6502.Arithmetic.cpp
    Cpu/Mos6502/Mos6502.Branches.cpp
    Cpu/Mos6502/Mos6502.Flags.cpp
    Cpu/Mos6502/Mos6502.IncrementDecrement.cpp
    Cpu/Mos6502/Mos6502.Interrupts.cpp
    Cpu/Mos6502/Mos6502.JumpsCalls.cpp
    Cpu/Mos6502/Mos6502.LoadStore.cpp
    Cpu/Mos6502/Mos6502.Logical.cpp
    Cpu/Mos6502/Mos6502.RegisterTransfers.cpp
    Cpu/Mos6502/Mos6502.Shifts.cpp
    Cpu/Mos6502/Mos6502.StackOperations.cpp
    Cpu/Mos6502/Mos6502.System.cpp
    Cpu/Mos6502/Mos6502.Unofficial.cpp

    # Sharp SM83
    Cpu/SharpSm83/SharpSm83Common.h
    Cpu/SharpSm83/SharpSm83.Arith8.cpp
    Cpu/SharpSm83/SharpSm83.Arith16.cpp
    Cpu/SharpSm83/SharpSm83.Bits.cpp
    Cpu/SharpSm83/SharpSm83.Interrupts.cpp
    Cpu/SharpSm83/SharpSm83.Invalid.cpp
    Cpu/SharpSm83/SharpSm83.Jumps.cpp
    Cpu/SharpSm83/SharpSm83.Ld8.cpp
    Cpu/SharpSm83/SharpSm83.Ld16.cpp
    Cpu/SharpSm83/SharpSm83.MiscControl.cpp
)

target_include_directories(${test_name}
    PRIVATE ${CMAKE_SOURCE_DIR}/libplip
)

target_link_libraries(${test_name} PRIVATE
    plip
    Catch2::Catch2WithMain
)

catch_discover_tests(${test_name})
