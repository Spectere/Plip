include(TestBigEndian)
TEST_BIG_ENDIAN(BIG_ENDIAN)

add_custom_target(
    GENERATE_LIB_VERSION_HEADER
    COMMAND ${CMAKE_COMMAND}
    -Dcmake_root="${CMAKE_CURRENT_SOURCE_DIR}"
    -Dsrc_root="${CMAKE_CURRENT_SOURCE_DIR}"
    -DPRODUCT_NAME="${product_name}"
    -DBIN_NAME="${lib_name}"
    -DVERSION_FILE="PlipVersion.h"
    -P "${CMAKE_CURRENT_SOURCE_DIR}/version.cmake"
)

add_library(${lib_name}
    DebugAudioChannel.h
    DebugValue.h
    PlipEmulationException.h
    PlipError.h
    PlipInitializationException.h
    PlipInstance.cpp                                PlipInstance.h
    PlipInvalidOpcodeException.h
    PlipIo.cpp                                      PlipIo.h
    PlipKeyValuePairCollection.h
    PlipSupport.h
    PlipUtility.cpp                                 PlipUtility.h

    Audio/PlipAudio.h

    Cpu/PlipCpu.cpp                                 Cpu/PlipCpu.h

    Input/PlipInput.cpp                             Input/PlipInput.h
    Input/PlipInputDefinition.cpp                   Input/PlipInputDefinition.h

    Memory/PlipMemory.h
    Memory/PlipMemoryMap.cpp                        Memory/PlipMemoryMap.h
    Memory/PlipMemoryRam.cpp                        Memory/PlipMemoryRam.h
    Memory/PlipMemoryRom.cpp                        Memory/PlipMemoryRom.h

    Video/PlipVideo.cpp                             Video/PlipVideo.h
    Video/PlipVideoException.h

    ########
    # CPUs #
    ########

    # CHIP-8
    Cpu/Chip8/Chip8.cpp                             Cpu/Chip8/Chip8.h
    Cpu/Chip8/Chip8.Ops.cpp

    # MOS 6502
    Cpu/Mos6502/Mos6502.cpp                         Cpu/Mos6502/Mos6502.h
    Cpu/Mos6502/Mos6502Registers.h
    Cpu/Mos6502/Mos6502.Ops.cpp

    # Sharp LR35902
    Cpu/SharpSm83/SharpSm83.cpp                     Cpu/SharpSm83/SharpSm83.h
    Cpu/SharpSm83/SharpSm83Registers.h
    Cpu/SharpSm83/SharpSm83.Ops.cpp


    #########
    # Cores #
    #########

    # CHIP-8
    Core/PlipCore.cpp                               Core/PlipCore.h
    Core/Chip8/Chip8Instance.cpp                    Core/Chip8/Chip8Instance.h

    # GameBoy
    Core/GameBoy/AudioChannel.h
    Core/GameBoy/DmaState.h
    Core/GameBoy/GameBoyInstance.cpp                Core/GameBoy/GameBoyInstance.h
    Core/GameBoy/GameBoyInstance.Audio.cpp
    Core/GameBoy/GameBoyInstance.Video.cpp
    Core/GameBoy/GameBoyIoRegisters.cpp             Core/GameBoy/GameBoyIoRegisters.h
    Core/GameBoy/GameBoyMapper.cpp                  Core/GameBoy/GameBoyMapper.h
    Core/GameBoy/GameBoyModel.h
    Core/GameBoy/Mbc2Ram.cpp                        Core/GameBoy/Mbc2Ram.h
    Core/GameBoy/MBC_Type.h
    Core/GameBoy/PPU_Mode.h
    Core/GameBoy/PPU_OutputStage.h

    # NES
    Core/Nes/NesApuRegisters.cpp                    Core/Nes/NesApuRegisters.h
    Core/Nes/NesInstance.cpp                        Core/Nes/NesInstance.h
    Core/Nes/NesInstance.Video.cpp
    Core/Nes/NesMapper.cpp                          Core/Nes/NesMapper.h
    Core/Nes/NesMemory.cpp                          Core/Nes/NesMemory.h
    Core/Nes/NesPpuRegisters.cpp                    Core/Nes/NesPpuRegisters.h
    Core/Nes/NesTiming.h
    Core/Nes/Mapper/M001_NROM.cpp                   Core/Nes/Mapper/M001_NROM.h
)

if(BIG_ENDIAN)
    target_compile_definitions(${lib_name} PRIVATE CORE_BIG_ENDIAN)
endif()

add_dependencies(${lib_name} GENERATE_LIB_VERSION_HEADER)
