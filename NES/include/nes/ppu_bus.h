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
    BYTE ram[2048];
    BYTE paletteIndexes[32];

    Cartridge * cartridge;

    void writeToPalette(WORD address, BYTE value);
  };
}