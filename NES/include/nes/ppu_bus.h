#pragma once

#include <nes\common\globals.h>

namespace nes {

  class Cartridge;

  class PpuBus {
  public:

    PpuBus(Cartridge & cartridge);

    void write(WORD address, BYTE value);
    BYTE readByte(WORD address);
    WORD readWord(WORD address);

  private:
    BYTE ram[2048];
    BYTE paletteIndexes[32];

    Cartridge * cartridge;
  };
}