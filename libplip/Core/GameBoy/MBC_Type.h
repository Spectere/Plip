/* MemoryBankController.h
 *
 * An enumeration containing a list of MBCs that can exist on GameBoy cartridges.
 */

#pragma once

namespace Plip::Core::GameBoy {
    enum class MBC_Type {
        None,
        Mbc1,
        Mbc2,
        Mbc3,
        Mbc5,
        Mbc6,
        Mbc7,
        Mmm01,
        PocketCamera,
        BandaiTama5,
        HuC1,
        HuC3
    };
}
