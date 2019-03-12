#pragma once

#include <nes/common/globals.h>

namespace nes {

  class Ppu;
  class Cartridge;
  
  class CpuBus {
  public:

    CpuBus(Ppu & ppu, Cartridge & cartridge);

    void write(WORD address, BYTE value);
    BYTE read(WORD address);
    WORD read16(WORD address);

  private:
    BYTE ram[2048];

    Ppu * ppu;
    //Apu * apu;
    Cartridge * cartridge;
  };
}
