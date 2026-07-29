/* GameBoyInstance.h
 *
 * A Game Boy emulation core.
 */

#pragma once

#include <filesystem>
#include <vector>

#include "DmaStateOam.h"
#include "GameBoyIoRegisters.h"
#include "GameBoyMapper.h"
#include "GameBoyModel.h"
#include "MBC_Type.h"
#include "PPU_Mode.h"
#include "PPU_OutputStage.h"
#include "Core/PlipCore.h"
#include "Cpu/SharpSm83/SharpSm83.h"
#include "Memory/PlipMemoryRam.h"
#include "Memory/PlipMemoryRom.h"

namespace Plip::Core::GameBoy {
    class GameBoyInstance final : public PlipCore {
    public:
        GameBoyInstance(PlipAudio* audio, PlipInput* input, PlipVideo* video, const PlipKeyValuePairCollection &config);
        ~GameBoyInstance() override;

        void Delta(double ns) override;
        [[nodiscard]] std::map<std::string, DebugValue> GetCpuRegisters() override { return m_cpu->GetRegisters(); }
        [[nodiscard]] std::vector<DebugAudioChannel> GetDebugAudioChannels() override;
        [[nodiscard]] std::map<std::string, std::map<std::string, DebugValue>> GetDebugInfo() const override;
        static std::string GetDmaStateOamString(DmaStateOam state);
        static std::string GetDmaTransferModeString(DmaTransferMode mode);
        [[nodiscard]] uint64_t GetLastExecutedOpcode() const override { return m_cpu->GetLastOp(); }
        [[nodiscard]] std::vector<uint64_t> GetPcs() const override;
        [[nodiscard]] uint64_t GetTotalCpuCycles() const override { return m_tCycleCount; }
        [[nodiscard]] uint64_t GetTotalVBlankCount() const override { return m_totalVBlankCount; }
        PlipError Load(const std::string &path) override;
        void Reset() override;
        void Shutdown() override;

        // GameBoyInstance.Dma
        void DMA_OAM_InitiateTransfer();

    private:
        struct PPU_Object {
            uint8_t Y;
            uint8_t X;
            uint8_t Index;
            uint8_t Flags;
        };

        // GameBoyInstance
        void BootRomFlagHandler();
        [[nodiscard]] int GetCartridgeRamBankCount() const;
        [[nodiscard]] bool IsMultiRomCartridge() const;
        void ReadJoypad();
        void ReadCartridgeFeatures();
        void RegisterInput() const;
        void RegisterWriteServiced() const;

        // GameBoyInstance.Audio
        static float APU_Clock_Channel(PulseChannel* channel);
        static float APU_Clock_Channel(WaveChannel* channel);
        static float APU_Clock_Channel(NoiseChannel* channel);
        void APU_Cycle();
        void APU_Dump();
        void APU_Init();
        void APU_Send();

        // GameBoyInstance.Dma
        void DMA_OAM_Cycle();
        void DMA_OAM_Finalize();
        void DMA_OAM_SetMemoryAccessibility(bool value) const;

        // GameBoyInstance.Video
        void PPU_Cycle();
        void PPU_DotClock(uint8_t lcdControl, uint8_t lcdStatus);
        bool PPU_DotClock_OamScan();
        bool PPU_DotClock_Output(uint8_t lcdControl);
        void PPU_DotClock_Output_Drawing(uint8_t lcdControl) const;
        [[nodiscard]] int PPU_DrawBackgroundOrWindow(uint32_t pixelOffset, bool isWindow, uint8_t palette, int offsetX, int offsetY, uint16_t tileMapAddress, uint16_t
                                                     tileDataAddress0, uint16_t tileDataAddress1) const;
        [[nodiscard]] bool PPU_DrawObject(uint32_t pixelOffset, PPU_Object object, bool tallSprites, int thisBgColor, bool lcdcPriority) const;
        void PPU_FinishTransition(uint8_t lcdStatus);
        void PPU_FinishTransition_OamScan(uint8_t lcdStatus);
        void PPU_FinishTransition_VBlank(uint8_t lcdStatus);
        [[nodiscard]] std::map<std::string, DebugValue> PPU_GetDebugInfo() const;
        void PPU_Plot_CGB(bool objPalette, int palette, int color, int pos) const;
        void PPU_Plot_DMG(int color, int pos) const;
        void PPU_Reset();
        static uint8_t PPU_ScaleColorChannel(uint8_t val);
        void PPU_SetMemoryPermissions() const;
        void PPU_VideoModeTransition();

        //
        // Fields
        //

        // PlipAudio
        std::vector<float> m_audioBuffer {};
        int m_audioSampleRate {};
        float m_audioResamplePos {};
        float m_audioCyclesPerSample {};
        float m_audioAccumulator {};
        int m_apuOutputSendThreshold {};

        // Core
        static constexpr uint32_t BaseClockRate = 4194304;
        static constexpr uint32_t ScreenWidth = 160;
        static constexpr uint32_t ScreenHeight = 144;
        static constexpr int HBlankDmaBatchLength = 0x10;
        static constexpr int OamDmaLength = 160;
        static constexpr int HeaderLogoOffset = 0x104;

        bool m_cgbMode {};
        Cpu::SharpSm83 *m_cpu {};
        double m_deltaTimeRemaining {};
        bool m_doubleSpeed {};
        GameBoyModel m_model;
        uint64_t m_tCycleCount {};
        uint64_t m_totalVBlankCount {};
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
        static constexpr auto LogoSize = 48;
        static constexpr uint8_t Logo[LogoSize] = {
            0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B, 0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D,
            0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E, 0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99,
            0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC, 0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E,
        };

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
        uint32_t m_cartRomBanks {};

        // System memory map
        GameBoyMapper* m_gbMemory {};

        PlipMemoryRom* m_bootRom;
        PlipMemoryRom* m_cartRom = nullptr;
        PlipMemoryRam* m_videoRam;
        PlipMemoryRam* m_workRam;
        PlipMemoryRam* m_oam = new PlipMemoryRam(0xA0, 0xFF);
        GameBoyIoRegisters* m_ioRegisters;
        PlipMemoryRam* m_highRam = new PlipMemoryRam(0x80, 0xFF);
        PlipMemory* m_cartRam = nullptr;

        // System flags
        bool m_bootRomDisableFlag = false;

        // DMA
        static constexpr int DmaOamByteCount = 0xA0;

        int m_dmaOamInitCycles = -1;
        uint16_t m_dmaOamStartAddress {};
        DmaStateOam m_dmaOamState = DmaStateOam::Inactive;
        int m_dmaOamPtr {};

        // APU
        constexpr static float ApuMixDivisor = 60;  // 15 audible volume levels, 4 channels
        constexpr static auto ApuClockDivisorNormal = 2;
        constexpr static auto ApuClockDivisorDouble = 4;
        constexpr static auto ApuFinalMixMultiplier = 0.60f;

        int m_apuClockDivisor = 2;

        PulseChannel* m_channel1 {};
        PulseChannel* m_channel2 {};
        WaveChannel* m_channel3 {};
        NoiseChannel* m_channel4 {};

        std::array<bool, 4> m_channelEnable { true, true, true, true };

        float m_channel1Last {};
        float m_channel2Last {};
        float m_channel3Last {};
        float m_channel4Last {};

        // PPU
        static constexpr auto PPU_Block0 = 0x0000;
        static constexpr auto PPU_Block1 = 0x0800;
        static constexpr auto PPU_Block2 = 0x1000;

        static constexpr auto PPU_OamScanTime = 80;
        static constexpr auto PPU_ScanlineTime = 456;
        static constexpr auto PPU_LyRolloverClock = 4;
        static constexpr auto PPU_Scanlines = 154;

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

        PlipMemoryRam* m_ppuCgbBgPaletteRam = new PlipMemoryRam(64, 0xFF);
        PlipMemoryRam* m_ppuCgbObjPaletteRam = new PlipMemoryRam(64, 0xFF);
        int m_ppuDotClock {};
        int m_ppuDmaCyclesRemaining = -1;
        int m_ppuDrawTime {};
        uint8_t m_ppuLastLcdControl {};
        bool m_ppuLcdOff = false;
        bool m_ppuLyc {};
        PPU_Mode m_ppuMode = PPU_Mode::OamScan;
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
        bool m_ppuSkip {};

#ifndef NDEBUG
        static constexpr auto PPU_DBG_TotalDotClocksPerFrame = 70224;
        int m_DBG_ppuFrameDotClocks {};
#endif // !NDEBUG

        // Timer
        uint16_t m_timerSystem {};
        bool m_timaQueueReload {};
        bool m_timerBitLast {};
    };
}
