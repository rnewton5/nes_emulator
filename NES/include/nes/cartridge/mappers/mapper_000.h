#pragma once

#include <nes/cartridge/mappers/mapper.h>

namespace nes {

  class Mapper000 : public Mapper {
  public:
    BYTE read(WORD address, Cartridge* cartridge) override;
    void write(WORD address, BYTE value, Cartridge* cartridge) override;
  };

}