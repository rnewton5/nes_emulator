#pragma once

#include <nes/cartridge/mappers/mapper.h>

namespace nes {

  class MapperFactory {
  public:
    static Mapper * Create(int mapperNumber);

  private:
    MapperFactory();
  };

}