#pragma once

#include <nes\common\globals.h>

namespace nes {

  class Ppu;
  class Cartridge;
  
  class CpuBus {
  public:

    CpuBus(Ppu & ppu, Cartridge & cartridge);

    void write(WORD address, BYTE value);
    BYTE readByte(WORD address);
    WORD readWord(WORD address);

  private:
    Ppu * ppu;
    //Apu * apu;
    Cartridge * cartridge;
  };

}
