/* GameBoyInstance.Dma.cpp
 *
 * (Color) Game Boy DMA transfers.
 */

#include "GameBoyInstance.h"

using Plip::Core::GameBoy::GameBoyInstance;

void GameBoyInstance::DMA_Reset() {
    m_vdmaMode = DmaModeVdma::Inactive;
    m_vdmaLength = m_vdmaOffset = m_vdmaSrcAddr = m_vdmaDestAddr = 0;
    m_vdmaScanlineComplete = false;

    m_vdmaBlockCpu = false;
}

//
// OAM DMA
//
void GameBoyInstance::DMA_OAM_Cycle() {
    switch(m_dmaOamState) {
        case DmaStateOam::Preparing: {
            if(--m_dmaOamInitCycles == 0) {
                // Set up DMA.
                m_dmaOamState = DmaStateOam::Transferring;
                auto startAddrHighByte = m_ioRegisters->DMA_GetOamAddress();

                if(startAddrHighByte >= 0xFE) {
                    // The DMA controller only drives VRAM and the external bus. This hack exists to make it
                    // work with our flat memory model. The *correct* solution would be to emulate the various
                    // memory buses. Maybe some day. :)
                    // The hack here is to simply pop off A13, effectively sending us back to WRAM.
                    startAddrHighByte &= 0xDF;
                }

                m_dmaOamStartAddress = startAddrHighByte << 8;
                m_dmaOamPtr = 0;

                // Set memory accessibility.
                DMA_OAM_SetMemoryAccessibility(false);
            }
            break;
        }

        case DmaStateOam::Transferring: {
            const auto val = m_memory->GetByte(m_dmaOamStartAddress + m_dmaOamPtr, true);
            m_oam->SetByte(m_dmaOamPtr, val, true);

            if(++m_dmaOamPtr == DmaOamByteCount) {
                DMA_OAM_Finalize();
            }
            break;
        }

        case DmaStateOam::Inactive:
        default:
            break;
    }
}

void GameBoyInstance::DMA_OAM_Finalize() {
    m_dmaOamState = DmaStateOam::Inactive;
    DMA_OAM_SetMemoryAccessibility(true);
}

void GameBoyInstance::DMA_OAM_InitiateTransfer() {
    // This will immediately be decremented after the cycle that sets the register.
    // Set this to 2 to compensate for that.
    m_dmaOamInitCycles = 2;
    m_dmaOamState = DmaStateOam::Preparing;
}

void GameBoyInstance::DMA_OAM_SetMemoryAccessibility(const bool value) const {
    m_videoRam->SetReadable(value);
    m_videoRam->SetWritable(value);

    m_oam->SetReadable(value);
    m_oam->SetWritable(value);

    if(m_model == GameBoyModel::CGB) {
        // CGB keeps the cartridge and WRAM on separate busses.
        if(m_dmaOamStartAddress < 0x8000) {
            m_cartRom->SetReadable(value);

            if(m_cartRamBanks > 0) {
                m_cartRam->SetReadable(value);
                m_cartRam->SetWritable(value);
            }
        }
        if(m_dmaOamStartAddress >= 0xC000 && m_dmaOamStartAddress < 0xFE00) {
            m_workRam->SetReadable(value);
            m_workRam->SetWritable(value);
        }
    } else {
        m_workRam->SetReadable(value);
        m_workRam->SetWritable(value);

        m_cartRom->SetReadable(value);

        if(m_cartRamBanks > 0) {
            m_cartRam->SetReadable(value);
            m_cartRam->SetWritable(value);
        }
    }
}

//
// VDMA (General Purpose/HBlank DMA -- CGB Only)
//
void GameBoyInstance::DMA_VDMA_Cycle() {
    switch(m_vdmaMode) {
        case DmaModeVdma::Inactive:
            break;
        case DmaModeVdma::GeneralPurpose:
            DMA_VDMA_Cycle_GDMA();
            break;
        case DmaModeVdma::HBlank:
            DMA_VDMA_Cycle_HDMA();
            break;
        default:
            throw PlipEmulationException("Invalid VDMA mode.");
    }
}

void GameBoyInstance::DMA_VDMA_Cycle_GDMA() {
    // GDMA copies indiscriminately until it's finished.
    const auto val = m_memory->GetByte((m_vdmaSrcAddr + m_vdmaOffset) & 0xFFFF, true);
    DMA_VDMA_VramWrite(m_vdmaDestAddr + m_vdmaOffset, val);

    if(++m_vdmaOffset >= m_vdmaLength) {
        DMA_VDMA_Finalize();
    }
}

void GameBoyInstance::DMA_VDMA_Cycle_HDMA() {
    // HDMA copies 16 bytes per HBlank, unless cancelled.
    if(m_cpu->IsHalted()) return;  // CPU is halted. Don't do anything.

    if(m_vdmaMode == DmaModeVdma::HBlank && m_ppuMode != PPU_Mode::HBlank) {
        m_vdmaScanlineComplete = false;
        return;
    }

    if(const auto lcdc = m_ioRegisters->GetByte(IoRegister::LcdControl); BIT_TEST(lcdc, 7) && m_vdmaScanlineComplete) {
        // Wait for the next HBlank (unless the LCD's disabled--if it is, keep copying).
        m_vdmaBlockCpu = false;
        return;
    }

    // Start copying.
    m_vdmaBlockCpu = true;

    const auto val = m_memory->GetByte((m_vdmaSrcAddr + m_vdmaOffset) & 0xFFFF, true);
    DMA_VDMA_VramWrite(m_vdmaDestAddr + m_vdmaOffset, val);

    ++m_vdmaOffset;
    if(m_vdmaOffset >= m_vdmaLength) {
        DMA_VDMA_Finalize();
    } else if((m_vdmaOffset & 0xF) == 0) {
        // 16 bytes copied. Wait for the next HBlank.
        m_vdmaScanlineComplete = true;
    }
}

void GameBoyInstance::DMA_VDMA_Finalize() {
    m_vdmaBlockCpu = false;
    m_vdmaMode = DmaModeVdma::Inactive;
}

void GameBoyInstance::DMA_VDMA_InitiateTransfer(const DmaModeVdma mode, const uint16_t srcAddr, const uint16_t destAddr, const int length) {
    m_vdmaMode = mode;
    m_vdmaLength = length;
    m_vdmaOffset = 0;
    m_vdmaSrcAddr = srcAddr;
    m_vdmaDestAddr = destAddr;
    m_vdmaScanlineComplete = false;

    if(m_vdmaMode == DmaModeVdma::GeneralPurpose) {
        // Execution is blocked while GDMA is performed.
        m_vdmaBlockCpu = true;
    }
}
