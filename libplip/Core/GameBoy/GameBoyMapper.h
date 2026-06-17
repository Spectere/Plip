/* GameBoyMapper.h
 *
 * A custom PlipMemoryMap that emulates the Game Boy's MBC chips.
 */

#pragma once

#include <fstream>

#include "GameBoyIoRegisters.h"
#include "MBC_Type.h"
#include "../../Memory/PlipMemory.h"
#include "../../Memory/PlipMemoryMap.h"
#include "../../Memory/PlipMemoryRom.h"

namespace Plip::Core::GameBoy {
    class GameBoyMapper final : public PlipMemoryMap {
    public:
        static constexpr auto RomBank0Address = 0x0000;
        static constexpr auto RomBank0Length = 0x4000;
        static constexpr auto RomBank1Address = 0x4000;
        static constexpr auto RomBank1Length = 0x4000;
        static constexpr auto VideoRamAddress = 0x8000;
        static constexpr auto VideoRamLength = 0x2000;
        static constexpr auto WorkRamAddress = 0xC000;
        static constexpr auto WorkRamLength = 0x2000;
        static constexpr auto CartRamAddress = 0xA000;
        static constexpr auto CartRamLength = 0x2000;
        static constexpr auto EchoRamAddress = 0xE000;
        static constexpr auto OamAddress = 0xFE00;
        static constexpr auto UnusableAddress = 0xFEA0;
        static constexpr auto IoRegistersAddress = 0xFF00;
        static constexpr auto HighRamAddress = 0xFF80;

        GameBoyMapper(PlipMemory* bootRom, PlipMemory* cartRom, PlipMemory* videoRam, PlipMemory* workRam, PlipMemory* oam, PlipMemory* ioRegisters, PlipMemory* highRam);

        void AssignBlock(PlipMemory* memory, uint32_t address, uint32_t offset = 0) override;
        void AssignBlock(PlipMemory* memory, uint32_t address, uint32_t offset, uint32_t length) override;
        PlipMemory* ConfigureMapper(MBC_Type mbcType, bool hasRtc, int cartRamBanks);
        void DisableBootRom();
        void EnableCartridgeRam(bool enable);
        [[nodiscard]] uint8_t GetByte(uint32_t address, bool privileged) const override;
        uint32_t GetLength() override { return 0x10000; }  // 64 KiB
        [[nodiscard]] std::map<std::string, DebugValue> GetMbcDebugInfo() const;
        void Reset();
        void RestoreCartridgeMemoryAccessibility() const;
        void SetByte(uint32_t address, uint8_t value, bool privileged = false) override;
        bool SetByte_HuC1(uint32_t address, uint8_t value);
        bool SetByte_Mbc1(uint32_t address, uint8_t value);
        bool SetByte_Mbc2(uint32_t address, uint8_t value);
        bool SetByte_Mbc3(uint32_t address, uint8_t value);
        bool SetByte_Mbc5(uint32_t address, uint8_t value);
        void SetVideoRamBank(int bank);
        void SetWorkRamBank(int bank);
        void UnassignAllBlocks() override;
        void UnassignBlock(uint32_t address, uint32_t length) override;

        void RTC_Clock();
        void RTC_Dump(std::fstream& file) const;
        void RTC_Increment();
        void RTC_LatchRegisters();
        void RTC_Load(std::fstream& file);
        [[nodiscard]] uint8_t RTC_RegisterGet(int index) const;
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

        bool m_largeBootRom {};

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
        uint8_t m_bankRegister2 {};
        bool m_bootRomAccessible {};
        bool m_cartHasRam {};
        uint8_t m_cartRamBank {};
        int m_cartRamBankCount {};
        uint32_t m_cartRomBankCount = 0;
        bool m_hasRtc {};
        bool m_hucIrMode {};
        std::string m_mbcName = "UNKNOWN";
        MBC_Type m_mbcType = MBC_Type::None;
        bool m_ramEnabled {};
        uint8_t m_rom0Bank {};
        uint16_t m_rom1Bank {};
        bool m_register1SelectsRomBank {};
        int m_vramBank {};
        int m_wramBank {};

        // RTC
        RtcRegisters m_rtcRegisters {};
        RtcRegisters m_rtcLatchedRegisters {
            .Days = 0x00,
            .Flags = 0x00,
            .Hours = 0x00,
            .Minutes = 0x00,
            .Seconds = 0x00
        };
        int m_rtcCpuClockRate {};
        int m_rtcMachineCycles {};
        uint8_t m_rtcLatchLastValueWritten {};
    };
}
