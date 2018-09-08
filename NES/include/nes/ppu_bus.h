#pragma once

#include <nes\common\globals.h>

namespace nes {

  class Cartridge;

  class PpuBus {
  public:

    PpuBus(Cartridge & cartridge);

    void write(WORD address, BYTE value);
    BYTE read(WORD address);
    WORD read16(WORD address);

  private:

    // The NES's ppu only has 2KiB of on board vram, but for simplicity I just
    // supplied 4KiB of vram. Mirroring will still work the same, and if a game
    // uses 4 screen mirroring I won't have to worry about using the vram on
    // the cartridge instead.
    BYTE ram[4096];
    BYTE paletteIndexes[32];

    Cartridge * cartridge;

    void writeToPalette(WORD address, BYTE value);
  };
}