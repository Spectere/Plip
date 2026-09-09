/* GameBoyInstance.h
 *
 * A Game Boy emulation core.
 */

#pragma once

#include <deque>
#include <filesystem>
#include <vector>

#include "Core/PlipCore.h"
#include "Cpu/SharpSm83/SharpSm83.h"
#include "DmaModeVdma.h"
#include "DmaStateOam.h"
#include "GameBoyIoRegisters.h"
#include "GameBoyMapper.h"
#include "GameBoyModel.h"
#include "MBC_Type.h"
#include "Memory/PlipMemoryRam.h"
#include "Memory/PlipMemoryRom.h"
#include "PPU_FetchTile.h"
#include "PPU_FetcherStage.h"
#include "PPU_FifoPixel.h"
#include "PPU_Mode.h"
#include "PPU_Object.h"

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
        static std::string GetDmaTransferModeString(DmaModeVdma mode);
        [[nodiscard]] uint64_t GetLastExecutedOpcode() const override { return m_cpu->GetLastOp(); }
        [[nodiscard]] std::vector<uint64_t> GetPcs() const override;
        [[nodiscard]] uint64_t GetTotalCpuCycles() const override { return m_tCycleCount; }
        [[nodiscard]] uint64_t GetTotalVBlankCount() const override { return m_totalVBlankCount; }
        PlipError Load(const std::string &path) override;
        void Reset() override;
        void Shutdown() override;

        // GameBoyInstance.Dma
        void DMA_OAM_InitiateTransfer();

        void DMA_VDMA_CancelTransfer() { DMA_VDMA_Finalize(); }
        DmaModeVdma DMA_VDMA_GetMode() const { return m_vdmaMode; }
        int DMA_VDMA_GetRemaining() const { return m_vdmaLength - m_vdmaOffset; }
        void DMA_VDMA_InitiateTransfer(DmaModeVdma mode, uint16_t srcAddr, uint16_t destAddr, int length);

    private:
        // GameBoyInstance
        void BootRomFlagHandler();
        [[nodiscard]] int GetCartridgeRamBankCount() const;
        [[nodiscard]] bool IsMultiRomCartridge() const;
        void ReadJoypad();
        void ReadCartridgeFeatures();
        void RegisterInput() const;
        void RegisterWriteServiced() const;
        void UpdateSharedPermissions() const;

        // GameBoyInstance.Audio
        static float APU_Clock_Channel(PulseChannel* channel);
        static float APU_Clock_Channel(WaveChannel* channel);
        static float APU_Clock_Channel(NoiseChannel* channel);
        void APU_Cycle();
        void APU_Dump();
        void APU_Init();
        void APU_Send();

        // GameBoyInstance.Dma
        void DMA_Reset();

        void DMA_OAM_Cycle();
        void DMA_OAM_Finalize();
        void DMA_OAM_SetMemoryAccessibility(bool value);

        void DMA_VDMA_Cycle();
        void DMA_VDMA_Cycle_GDMA();
        void DMA_VDMA_Cycle_HDMA();
        void DMA_VDMA_Finalize();
        void DMA_VDMA_VramWrite(const uint32_t addr, const uint8_t val) const {
            const auto vramAddr = (m_gbMemory->GetVramBank() * 0x2000) + (addr % 0x2000);
            m_videoRam->SetByte(vramAddr, val, true);
        }

        // GameBoyInstance.Video
        static uint8_t PPU_ScaleColorChannel(const uint8_t val) { return (val << 3) | (val >> 2); }
        [[nodiscard]] std::map<std::string, DebugValue> PPU_GetDebugInfo() const;
        void PPU_Plot_CGB(bool objPalette, int palette, int color, int pos) const;
        void PPU_Plot_DMG(int color, int pos) const;
        void PPU_Reset();
        void PPU_SetMemoryPermissions();

        void PPU_Cycle();
        void PPU_DisableLcdCheck(uint8_t lcdControl);
        void PPU_Fetcher_Clock();
        void PPU_Fetcher_Finish();
        void PPU_Fetcher_Setup(uint8_t lcdControl);
        void PPU_Fetcher_OBJ_Scan();
        uint16_t PPU_GetFetchTileIndex() const;
        void PPU_Mode_OamScan(uint8_t lcdControl, uint8_t lcdStatus);
        void PPU_Mode_Output(uint8_t lcdControl, uint8_t lcdStatus);
        void PPU_UpdateRegisters(uint8_t lcdStatus);
        void PPU_TransitionMode(uint8_t lcdStatus);
        void PPU_TransitionStatInterrupt(uint8_t lcdStatus) const;

        uint16_t PPU_GetBaseBgTileAddr(const uint8_t idx, const int lineOffset) const {
            const auto blockBaseAddress = (idx >= 0x80)
                ? PPU_Block1
                : (m_ppuBgUseBlock2 ? PPU_Block2 : PPU_Block0);
            return blockBaseAddress + ((idx % 128) * 16) + (lineOffset * 2);
        }

        static uint16_t PPU_GetBaseObjTileAddr(uint8_t idx, const int lineOffset, const bool tallSprite) {
            if(tallSprite) {
                // Ensure that the base tile for 8x16 sprites always has its LSB set to 0.
                // Lines >= 8 will naturally spill over to the next sprite in the return.
                idx &= 0b11111110;
            }

            return PPU_Block0 + (idx * 16) + (lineOffset * 2);
        }

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

        bool m_dmaOamAccess = true;
        bool m_dmaVramAccess = true;

        int m_dmaOamInitCycles = -1;
        uint16_t m_dmaOamStartAddress {};
        DmaStateOam m_dmaOamState = DmaStateOam::Inactive;
        int m_dmaOamPtr {};

        DmaModeVdma m_vdmaMode = DmaModeVdma::Inactive;
        bool m_vdmaBlockCpu {};
        int m_vdmaLength {};
        int m_vdmaOffset {};
        uint16_t m_vdmaSrcAddr {};
        uint16_t m_vdmaDestAddr {};
        bool m_vdmaScanlineComplete {};

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

        static constexpr auto PPU_FifoLength = 8;  // Pan Docs says 16. Others say 8. idk lol. 8 seems sufficient?

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

        // PPU: General
        PlipMemoryRam* m_ppuCgbBgPaletteRam = new PlipMemoryRam(64, 0xFF);
        PlipMemoryRam* m_ppuCgbObjPaletteRam = new PlipMemoryRam(64, 0xFF);
        int m_ppuDotClock {};
        bool m_ppuInitOutput {};
        uint8_t m_ppuLastLcdControl {};
        bool m_ppuLcdOff {};
        int m_ppuLcdXCoordinate {};
        int m_ppuFifoXCoordinate {};
        int m_ppuLcdYCoordinate {};
        uint8_t m_ppuScrollX {};
        uint8_t m_ppuScrollY {};
        PPU_Mode m_ppuMode = PPU_Mode::OamScan;
        bool m_ppuSkip {};
        int m_ppuDiscardBg {};
        bool m_ppuPlot {};

        // Tile fetcher
        std::array<uint8_t, 8> m_ppuFetcherResult {};
        bool m_ppuFetchObjMode {};
        bool m_ppuFetchObjPending {};
        bool m_ppuFetcherObjTall {};
        PPU_Object m_ppuFetcherObj {};
        PPU_FetcherStage m_ppuFetcherStage = PPU_FetcherStage::Idle;
        PPU_FetchTile m_ppuFetcherTile {};

        // Background / window
        bool m_ppuBgUseMap1 {};
        bool m_ppuBgUseBlock2 {};
        std::deque<PPU_FifoPixel> m_ppuFifoBg = std::deque<PPU_FifoPixel>(PPU_FifoLength);
        bool m_ppuBgPriority {};
        int m_ppuTileLine {};

        // OBJ
        int m_ppuOamScanIndex {};
        std::deque<PPU_Object> m_ppuObjectDrawList = std::deque<PPU_Object>(PPU_ObjectsPerScanline);
        std::deque<PPU_FifoPixel> m_ppuFifoObj = std::deque<PPU_FifoPixel>(PPU_FifoLength);

        // PPU RAM access
        bool m_ppuOamAccess = true;
        bool m_ppuVramAccess = true;

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
