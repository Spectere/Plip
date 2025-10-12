/* NesPpuRegisters.h
 *
 * Implements the NES's PPU registers.
 */

#pragma once

#include <array>
#include <map>
#include <string>

#include "../PlipCore.h"
#include "../../Cpu/Mos6502/Mos6502.h"
#include "../../Memory/PlipMemory.h"

namespace Plip::Core::Nes {
    enum class PpuRegister {
        PpuControl = 0x00,
        PpuMask = 0x01,
        PpuStatus = 0x02,
        OamAddress = 0x03,
        OamData = 0x04,
        PpuScroll = 0x05,
        PpuAddress = 0x06,
        PpuData = 0x07,
    };

    class NesPpuRegisters final : public PlipMemory {
    public:
        explicit NesPpuRegisters(PlipMemory* ppuRam) : m_ppuRam(ppuRam) {}
        
        [[nodiscard]] uint8_t GetByte(const uint32_t address, const bool privileged = false) override {
            return GetByte(static_cast<PpuRegister>(address), privileged);
        }

        void SetByte(const uint32_t address, const uint8_t value, const bool privileged = false) override {
            SetByte(static_cast<PpuRegister>(address), value, privileged);
        }

        [[nodiscard]] uint8_t GetByte(PpuRegister ppuRegister, bool privileged);
        [[nodiscard]] uint32_t GetLength() override;
        void Reset();
        void SetByte(PpuRegister ppuRegister, uint8_t value, bool privileged);

        [[nodiscard]] uint8_t GetByteOam(uint8_t address) const;
        void SetByteOam(uint8_t address, uint8_t value);

        [[nodiscard]] uint8_t GetBytePalette(const uint8_t address) const { return m_palette[address % 0x20]; }

        [[nodiscard]] uint16_t GetBaseNamespaceAddress() const { return 0x2000 + (m_ppuCtrlBaseNamespaceAddress * 0x400); }
        [[nodiscard]] uint16_t GetBackgroundPatternAddress() const { return m_ppuCtrlBackgroundAddressHigh ? 0x1000 : 0; }
        [[nodiscard]] std::map<std::string, DebugValue> GetDebugInfo() const;
        [[nodiscard]] bool GetOddFrame() const { return m_oddFrame; }
        [[nodiscard]] bool GetVBlankNmi() const { return m_ppuCtrlNmiEnabled; }

        void SetCpu(Cpu::Mos6502* cpu) { m_cpu = cpu; }
        
        void SetStatusSpriteOverflow(const bool value) { m_ppuStatusSpriteOverflow = value; } 
        void SetStatusSprite0Hit(const bool value) { m_ppuStatusSprite0Hit = value; }
        void SetStatusVBlank(const bool value) { m_ppuStatusVBlank = value; }

    private:
        void IncrementPpuAddress();

        //
        // Fields
        //
        static constexpr int m_oamSize = 256;
        std::array<uint8_t, m_oamSize> m_oam {};

        static constexpr int m_paletteSize = 0x20;
        std::array<uint8_t, m_paletteSize> m_palette {};

        Cpu::Mos6502* m_cpu {};
        PlipMemory* m_ppuRam;
        
        // PPUCTRL
        uint8_t m_ppuDebugCtrl {};
        uint8_t m_ppuCtrlBaseNamespaceAddress {};
        bool m_ppuCtrlVramIncrementVertical {};
        bool m_ppuCtrlSpriteTableAddressHigh {};
        bool m_ppuCtrlBackgroundAddressHigh {};
        bool m_ppuCtrlTallSprites {};
        bool m_ppuCtrlOutputToExt {};
        bool m_ppuCtrlNmiEnabled {};

        // PPUMASK
        uint8_t m_ppuDebugMask {};
        bool m_ppuMaskGreyscale {};
        bool m_ppuMaskDisableBackgroundMask {};
        bool m_ppuMaskDisableSpriteMask {};
        bool m_ppuMaskRenderBackground {};
        bool m_ppuMaskRenderSprites {};
        bool m_ppuMaskEmphasizeRed {};
        bool m_ppuMaskEmphasizeGreen {};
        bool m_ppuMaskEmphasizeBlue {};

        // PPUSTATUS
        bool m_ppuStatusSpriteOverflow = true;
        bool m_ppuStatusSprite0Hit {};
        bool m_ppuStatusVBlank = true;
        
        uint8_t m_ppuStatus = 0b10100000;
        uint8_t m_oamAddress {};
        uint8_t m_ppuScrollX {};
        uint8_t m_ppuScrollY {};
        uint16_t m_ppuAddress {};
        uint8_t m_ppuDataBuffer {};

        bool m_oddFrame {};

        int m_ppuRegWriteLatch {};  // internal register: w
    };
}
