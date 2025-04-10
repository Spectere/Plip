/* GameBoyMapper.h
 *
 * A custom PlipMemoryMap that emulates the Game Boy's MBC chips.
 */

#pragma once

#include <fstream>

#include "GameBoyIoRegisters.h"
#include "MBC_Type.h"
#include "../../Memory/PlipMemory.h"
#include "../../Memory/PlipMemoryRom.h"

namespace Plip::Core::GameBoy {
    class GameBoyMapper final : public PlipMemoryMap {
    public:
        static constexpr auto RomBank0Address = 0x0000;
        static constexpr auto RomBank0Length = 0x4000;
        static constexpr auto RomBank1Address = 0x4000;
        static constexpr auto RomBank1Length = 0x4000;
        static constexpr auto VideoRamAddress = 0x8000;
        static constexpr auto WorkRamAddress = 0xC000;
        static constexpr auto CartRamAddress = 0xA000;
        static constexpr auto CartRamLength = 0x2000;
        static constexpr auto EchoRamAddress = 0xE000;
        static constexpr auto OamAddress = 0xFE00;
        static constexpr auto UnusableAddress = 0xFEA0;
        static constexpr auto IoRegistersAddress = 0xFF00;
        static constexpr auto HighRamAddress = 0xFF80;
        
        GameBoyMapper(PlipMemory* bootRom, PlipMemory* cartRom, PlipMemory* videoRam, PlipMemory* workRam, PlipMemory* oam, PlipMemory* ioRegisters, PlipMemory* highRam);
        PlipMemory* ConfigureMapper(MBC_Type mbcType, bool hasRtc, int cartRamBanks);
        void DisableBootRom();
        void EnableCartridgeRam(bool enable);
        uint8_t GetByte(uint32_t address, bool privileged) const override;
        uint8_t GetByte_Mbc3(uint32_t address, bool privileged) const;
        std::map<std::string, DebugValue> GetMbcDebugInfo() const;
        void RemapMemory(bool remapRom, bool remapRam);
        void Reset();
        void RestoreCartridgeMemoryAccessibility() const;
        void SetByte(uint32_t address, uint8_t value, bool privileged = false) override;
        bool SetByte_Mbc1(uint32_t address, uint8_t value);
        bool SetByte_Mbc2(uint32_t address, uint8_t value);
        bool SetByte_Mbc3(uint32_t address, uint8_t value);

        void RTC_Clock();
        void RTC_Dump(std::fstream& file) const;
        void RTC_Increment();
        void RTC_LatchRegisters();
        void RTC_Load(std::fstream& file);
        uint8_t RTC_RegisterGet(int index) const;
        void RTC_RegisterSet(int index, uint8_t value);
        void RTC_ResetSubSecondClock();
        void RTC_SetCpuClockRate(int clockRate);

    private:
        struct RtcRegisters {
            uint8_t Days;
            uint8_t Flags;
            uint8_t Hours;
            uint8_t Minutes;
            uint8_t Seconds;
        };
        
        static constexpr uint8_t UnusableContents[0x60] {};
        PlipMemory* m_bootRom = nullptr;
        PlipMemory* m_cartRom = nullptr;
        PlipMemory* m_videoRam = nullptr;
        PlipMemory* m_workRam = nullptr;
        PlipMemory* m_oam = nullptr;
        PlipMemory* m_unusable = new PlipMemoryRom(UnusableContents, 0x60, 0xFF);
        PlipMemory* m_ioRegisters = nullptr;
        PlipMemory* m_highRam = nullptr;
        PlipMemory* m_cartRam = nullptr;

        uint8_t m_bankingMode {};
        uint8_t m_bankRegister0 {};
        uint8_t m_bankRegister1 {};
        bool m_cartHasRam {};
        int m_cartRamBanks {};
        bool m_hasRtc {};
        std::string m_mbcName = "UNKNOWN";
        MBC_Type m_mbcType = MBC_Type::None;
        uint8_t m_ramBank {};
        bool m_ramEnabled {};
        uint8_t m_rom0Bank {};
        uint8_t m_rom1Bank {};
        bool m_register1SelectsRomBank {};

        // RTC
        RtcRegisters m_rtcRegisters {};
        RtcRegisters m_rtcLatchedRegisters {
            .Days = 0xFF,
            .Flags = 0xFF,
            .Hours = 0xFF,
            .Minutes = 0xFF,
            .Seconds = 0xFF
        };
        int m_rtcCpuClockRate {};
        int m_rtcMachineCycles {};
        uint8_t m_rtcLatchLastValueWritten {};
    };
}
