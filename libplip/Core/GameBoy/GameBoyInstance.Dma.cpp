/* GameBoyInstance.Dma.cpp
 *
 * (Color) Game Boy DMA transfers.
 */

#include "GameBoyInstance.h"

using Plip::Core::GameBoy::GameBoyInstance;

void GameBoyInstance::DMA_OAM_Cycle() {
    if(m_tCycleCount % 4) return;

    switch(m_dmaOamState) {
        case DmaStateOam::Preparing: {
            if(--m_dmaOamInitCycles == 0) {
                // Set up DMA.
                m_dmaOamState = DmaStateOam::Transferring;
                m_dmaOamStartAddress = m_ioRegisters->DMA_GetOamAddress() << 8;
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
                DMA_Oam_Finalize();
            }
            break;
        }

        case DmaStateOam::Inactive:
        default:
            break;
    }
}

void GameBoyInstance::DMA_Oam_Finalize() {
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
