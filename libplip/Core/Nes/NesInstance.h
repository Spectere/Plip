/* NesInstance.h
 *
 * An NES emulation core.
 */

#pragma once

#include <filesystem>
#include <queue>
#include <vector>

#include "NesMemory.h"
#include "NesTiming.h"
#include "../PlipCore.h"
#include "../../Cpu/Mos6502/Mos6502.h"
#include "../../Memory/PlipMemoryRam.h"
#include "../../Memory/PlipMemoryRom.h"

namespace Plip::Core::Nes {
    class NesInstance final : public PlipCore {
    public:
        NesInstance(PlipAudio* audio, PlipInput* input, PlipVideo* video, const PlipKeyValuePairCollection &config);
        ~NesInstance() override;
        
        void Delta(long ns) override;
        std::map<std::string, std::map<std::string, DebugValue>> GetDebugInfo() const override;
        std::vector<uint64_t> GetPcs() const override;
        PlipError Load(const std::string& path) override;
        void Reset() override;

    private:
        bool ReadRomHeader(const std::vector<char>& headerData);
        void ReadRomHeaderINes(const std::vector<char>& headerData);
        void ReadRomHeaderNes2(const std::vector<char>& headerData);
        void RegisterInput() const;

        void ReadControllers();

        void PPU_CheckForNextScanline();
        void PPU_Cycle();
        void PPU_Cycle_FetchAndRender();
        void PPU_Cycle_VBlank();
        void PPU_Draw_Background(int pixelX);
        [[nodiscard]] uint8_t* PPU_GetColorPtr(int index);
        [[nodiscard]] std::map<std::string, DebugValue> PPU_GetDebugInfo() const;
        void PPU_ReadMemory(bool spriteQueue, bool holdStage = false);
        
        //
        // Fields
        //

        // Core
        static constexpr uint32_t MasterClockRateNtsc = 21477272;
        static constexpr uint32_t ClockDivisorNtsc = 12;
        static constexpr uint32_t CpuClockNtsc = MasterClockRateNtsc / ClockDivisorNtsc;
        
        static constexpr uint32_t MasterClockRatePal = 26601712;
        static constexpr uint32_t ClockDivisorPal = 16;
        static constexpr uint32_t CpuClockPal = MasterClockRatePal / ClockDivisorPal;

        static constexpr uint32_t MasterClockRateDendy = 2601712;
        static constexpr uint32_t ClockDivisorDendy = 15;
        static constexpr uint32_t CpuClockDendy = MasterClockRateDendy / ClockDivisorDendy;

        static constexpr uint32_t ScreenWidth = 256;
        static constexpr uint32_t ScreenHeight = 240;

        Cpu::Mos6502* m_cpu {};
        PlipVideoFormatInfo m_videoFormat {};
        uint8_t* m_videoBuffer;
        size_t m_videoBufferSize;

        // Cartridge
        std::filesystem::path m_cartPath {};
        std::filesystem::path m_batteryPath {};

        bool m_cartUsesINesHeader {};  // i.e. probably needs data from the mapper to be usable
                                       // (and even then there's no guarantee)

        bool m_cartNametableHorizontal {};
        bool m_cartHasBattery {};
        bool m_cartHasTrainer {};
        bool m_cartUsesAltNamespace {};

        int m_cartPrgRomSize {};
        int m_cartChrRomSize {};

        int m_cartPrgRamSize {};
        int m_cartChrRamSize {};

        int m_cartPrgNvramSize {};
        int m_cartChrNvramSize {};

        int m_cartMapper {};
        int m_cartSubmapper {};

        NesTiming m_cartTimingMode {};

        // Input
        static constexpr auto InputP1_A      = 0;
        static constexpr auto InputP1_B      = 1;
        static constexpr auto InputP1_Select = 2;
        static constexpr auto InputP1_Start  = 3;
        static constexpr auto InputP1_Up     = 4;
        static constexpr auto InputP1_Down   = 5;
        static constexpr auto InputP1_Left   = 6;
        static constexpr auto InputP1_Right  = 7;

        static constexpr auto InputP2_A      = 8;
        static constexpr auto InputP2_B      = 9;
        static constexpr auto InputP2_Select = 10;
        static constexpr auto InputP2_Start  = 11;
        static constexpr auto InputP2_Up     = 12;
        static constexpr auto InputP2_Down   = 13;
        static constexpr auto InputP2_Left   = 14;
        static constexpr auto InputP2_Right  = 15;

        // System memory
        static constexpr int m_trainerSize = 512;
        static constexpr int m_workRamAmount = 2 * 1024;  // 2KiB each
        static constexpr int m_ppuRamAmount  = 2 * 1024;
        
        NesMemory* m_nesMemory {};
        
        NesApuRegisters* m_apuRegisters {};
        NesPpuRegisters* m_ppuRegisters {};

        PlipMemoryRam* m_workRam {};
        PlipMemoryRam* m_ppuRam {};
        PlipMemoryRom* m_prgRom {};
        PlipMemoryRom* m_chrRom {};
        PlipMemoryRom* m_trainerRom {};

        NesMapper* m_mapper {};

        // Input
        uint8_t m_input1 = 0;
        uint8_t m_input2 = 0;

        // Video/PPU
        uint8_t m_ppuCurrentAttribute = 0;
        uint8_t m_ppuCurrentTileHigh = 0;
        uint8_t m_ppuCurrentTileLow = 0;
        int m_ppuScanlineCycle {};
        int m_ppuScanlineEnd = 340;
        int m_ppuScanlineY = -1;
        int m_ppuRegScrollPosition {};  // v
        int m_ppuRegScrollXCoarse {};   // t
        int m_ppuRegScrollXFine {};     // x
        std::queue<int> m_ppuSpriteQueue {};
        std::queue<int> m_ppuTileQueue {};
        int m_ppuReadStage = 0;
        int m_ppuNametablePointer = 0;
        int m_ppuNametablePointerPlotting = 0;
        int m_ppuLastPatternIndex = 0;

        std::array<uint8_t, 0x40 * 3> m_nesPalette = {
            117, 117, 117,  // $00
             36,  24, 142,  // $01
              0,   0, 170,  // $02
             69,   0, 158,  // $03
            142,   0, 117,  // $04
            170,   0,  16,  // $05
            166,   0,   0,  // $06
            125,   8,   0,  // $07
             65,  44,   0,  // $08
              0,  69,   0,  // $09
              0,  81,   0,  // $0A
              0,  60,  20,  // $0B
             24,  60,  93,  // $0C
              0,   0,   0,  // $0D
              0,   0,   0,  // $0E
              0,   0,   0,  // $0F
            190, 190, 190,  // $10
              0, 113, 239,  // $11
             32,  56, 239,  // $12
            130,   0, 243,  // $13
            190,   0, 190,  // $14
            231,   0,  89,  // $15
            219,  40,   0,  // $16
            203,  77,  12,  // $17
            138, 113,   0,  // $18
              0, 150,   0,  // $19
              0, 170,   0,  // $1A
              0, 146,  56,  // $1B
              0, 130, 138,  // $1C
              0,   0,   0,  // $1D
              0,   0,   0,  // $1E
              0,   0,   0,  // $1F
            255, 255, 255,  // $20
             60, 190, 255,  // $21
             93, 150, 255,  // $22
            207, 138, 255,  // $23
            247, 121, 255,  // $24
            255, 117, 182,  // $25
            255, 117,  97,  // $26
            255, 154,  56,  // $27
            243, 190,  60,  // $28
            130, 211,  16,  // $29
             77, 223,  73,  // $2A
             89, 251, 154,  // $2B
              0, 235, 219,  // $2C
            121, 121, 121,  // $2D
              0,   0,   0,  // $2E
              0,   0,   0,  // $2F
            255, 255, 255,  // $30
            170, 231, 255,  // $31
            199, 215, 255,  // $32
            215, 203, 255,  // $33
            255, 199, 255,  // $34
            255, 199, 219,  // $35
            255, 190, 178,  // $36
            255, 219, 170,  // $37
            255, 231, 162,  // $38
            227, 255, 162,  // $39
            170, 243, 190,  // $3A
            178, 255, 207,  // $3B
            158, 255, 243,  // $3C
            199, 199, 199,  // $3D
              0,   0,   0,  // $3E
              0,   0,   0,  // $3F

        };
    };
}
