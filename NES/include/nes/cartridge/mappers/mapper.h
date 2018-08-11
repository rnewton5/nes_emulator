#pragma once

#include <nes\common\globals.h>

namespace nes {

  class Cartridge;

  class Mapper {
  public:
    virtual void write(WORD address, BYTE value, Cartridge* cartridge) = 0;
    virtual BYTE read(WORD address, Cartridge* cartridge) = 0;

  protected:
    static const int CHR_STARTING_ADDRESS = 0x0000;
    static const int RAM_STARTING_ADDRESS = 0x6000;
    static const int ROM_STARTING_ADDRESS = 0x8000;
  };

}
