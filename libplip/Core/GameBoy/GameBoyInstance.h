/* GameBoyInstance.h
 *
 * A Game Boy emulation core.
 */

#pragma once

#include <filesystem>
#include <vector>

#include "GameBoyIoRegisters.h"
#include "GameBoyMapper.h"
#include "MBC_Type.h"
#include "PPU_Mode.h"
#include "PPU_OutputStage.h"
#include "../PlipCore.h"
#include "../../Cpu/SharpLr35902/SharpLr35902.h"
#include "../../Memory/PlipMemoryRam.h"
#include "../../Memory/PlipMemoryRom.h"

namespace Plip::Core::GameBoy {
    class GameBoyInstance final : public PlipCore {
    public:
        GameBoyInstance(PlipAudio* audio, PlipInput* input, PlipVideo* video, const PlipKeyValuePairCollection &config);
        ~GameBoyInstance() override;

        void Delta(long ns) override;
        [[nodiscard]] std::map<std::string, std::map<std::string, DebugValue>> GetDebugInfo() const override;
        std::vector<uint64_t> GetPcs() const override;
        PlipError Load(const std::string &path) override;
        void Reset() override;
        void Shutdown() override;

    private:
        struct PPU_Object {
            uint8_t Y;
            uint8_t X;
            uint8_t Index;
            uint8_t Flags;
        };

        // GameBoyInstance
        void BootRomFlagHandler();
        void CompleteOamDmaCopy() const;
        int GetCartridgeRamBankCount() const;
        void PerformOamDmaCopy(int sourceAddress);
        void ReadJoypad();
        void ReadCartridgeFeatures();
        void RegisterInput() const;
        void RegisterWriteServiced() const;

        // GameBoyInstance.Video
        void PPU_Cycle();
        void PPU_DotClock(uint8_t lcdControl, uint8_t lcdStatus);
        bool PPU_DotClock_OamScan();
        bool PPU_DotClock_Output(uint8_t lcdControl);
        void PPU_DotClock_Output_Drawing(uint8_t lcdControl);
        int PPU_DrawBackgroundOrWindow(uint32_t pixelOffset, bool isWindow, uint8_t palette, int offsetX, int offsetY, uint16_t tileMapAddress, uint16_t tileDataAddress0, uint16_t tileDataAddress1) const;
        bool PPU_DrawObject(uint32_t pixelOffset, PPU_Object object, bool tallSprites, int thisBgColor) const;
        void PPU_FinishTransition(uint8_t lcdStatus);
        void PPU_FinishTransition_OamScan(uint8_t lcdStatus);
        void PPU_FinishTransition_VBlank(uint8_t lcdStatus);
        [[nodiscard]] std::map<std::string, DebugValue> PPU_GetDebugInfo() const;
        void PPU_Plot(int color, int pos) const;
        void PPU_Reset();
        void PPU_SetMemoryPermissions() const;
        void PPU_VideoModeTransition();

        //
        // Fields
        //

        // Core
        static constexpr uint32_t BaseClockRate = 4194304;
        static constexpr uint32_t ScreenWidth = 160;
        static constexpr uint32_t ScreenHeight = 144;

        Cpu::SharpLr35902 *m_cpu;
        PlipVideoFormatInfo m_videoFormat {};
        uint8_t *m_videoBuffer;
        size_t m_videoBufferSize;

        // Input
        static constexpr auto InputA      = 0;
        static constexpr auto InputB      = 1;
        static constexpr auto InputSelect = 2;
        static constexpr auto InputStart  = 3;
        static constexpr auto InputRight  = 4;
        static constexpr auto InputLeft   = 5;
        static constexpr auto InputUp     = 6;
        static constexpr auto InputDown   = 7;

        int m_keypad {};

        // Cartridge features
        static constexpr auto CartRamSizeOffset = 0x0149;

        std::filesystem::path m_cartPath {};
        std::filesystem::path m_batteryPath {};
        std::filesystem::path m_rtcDataPath {};

        MBC_Type m_mbc = MBC_Type::None;
        bool m_hasBattery = false;
        bool m_hasCamera = false;
        bool m_hasCartRam = false;
        bool m_hasRtc = false;
        bool m_hasSensor = false;

        uint16_t m_cartRamBanks = 0;

        // System memory map
        GameBoyMapper* m_gbMemory;

        PlipMemoryRom* m_bootRom;
        PlipMemoryRom* m_cartRom = nullptr;
        PlipMemoryRam* m_videoRam = new PlipMemoryRam(0x2000, 0xFF);
        PlipMemoryRam* m_workRam = new PlipMemoryRam(0x2000, 0xFF);
        PlipMemoryRam* m_oam = new PlipMemoryRam(0x100, 0xFF);
        GameBoyIoRegisters* m_ioRegisters = new GameBoyIoRegisters();
        PlipMemoryRam* m_highRam = new PlipMemoryRam(0x80, 0xFF);
        PlipMemory* m_cartRam = nullptr;

        // System flags
        bool m_bootRomDisableFlag = false;

        // DMA
        int m_oamDmaDelayCycles = 0;
        int m_oamDmaSourceAddress = 0;

        // PPU
        static constexpr auto PPU_Block0 = 0x0000;
        static constexpr auto PPU_Block1 = 0x0800;
        static constexpr auto PPU_Block2 = 0x1000;
        static constexpr auto PPU_DotsPerCycle = 4;

        static constexpr auto PPU_OamScanTime = 80;
        static constexpr auto PPU_ScanlineTime = 456;
        static constexpr auto PPU_FrameTime = 4560;

        static constexpr auto PPU_TileBase = 0x0000;
        static constexpr auto PPU_TileBaseBlock2 = 0x1000;
        static constexpr auto PPU_TileMapBase = 0x1800;
        static constexpr auto PPU_TileMapBlockOffset = 0x0400;

        static constexpr auto PPU_TileSizeX = 8;
        static constexpr auto PPU_TileSizeY = 8;
        static constexpr auto PPU_MapTileCountX = 256 / PPU_TileSizeX;
        static constexpr auto PPU_MapTileCountY = 256 / PPU_TileSizeY;

        static constexpr auto PPU_MaximumObjectCount = 40;
        static constexpr auto PPU_ObjectsPerScanline = 10;

        int m_ppuDotClock {};
        int m_ppuDmaCyclesRemaining = -1;
        int m_ppuDrawTime {};
        uint8_t m_ppuLastLcdControl {};
        bool m_ppuLcdOff = false;
        bool m_ppuLyc {};
        PPU_Mode m_ppuMode {};
        bool m_ppuOamScanComplete = false;
        std::vector<PPU_Object> m_ppuObjectDrawList {};
        int m_ppuOutputClock {};
        PPU_OutputStage m_ppuOutputStage {};
        uint8_t m_ppuLcdXCoordinate {};
        uint8_t m_ppuLcdYCoordinate {};
        int m_ppuScrollX {};
        bool m_ppuWindowEnabled {};
        bool m_ppuWindowSetUp {};
        int m_ppuWindowX {};
        int m_ppuWindowY {};

        // Timer
        uint16_t m_timerSystem {};
        bool m_timaQueueReload {};
        bool m_timerBitLast {};
    };
}
