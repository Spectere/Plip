/* NesInstance.h
 *
 * An NES emulation core.
 */

#pragma once

#include <filesystem>
#include <vector>

#include "NesMemory.h"
#include "NesTiming.h"
#include "../../Cpu/Mos6502/Mos6502.h"
#include "../PlipCore.h"

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
        static constexpr uint32_t ScreenHeight = 242;

        Cpu::Mos6502 *m_cpu {};
        PlipVideoFormatInfo m_videoFormat {};
        uint8_t *m_videoBuffer;
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

        // System memory map
        NesMemory* m_nesMemory {};
    };
}
