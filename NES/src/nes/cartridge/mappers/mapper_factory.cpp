#include <nes\cartridge\mappers\mapper.h>
#include <nes\cartridge\mappers\mapper_factory.h>
#include <nes\cartridge\mappers\mapper_000.h>
#include <nes\cartridge\mappers\mapper_001.h>
#include <exception>

namespace nes {

  Mapper * MapperFactory::Create(int mapperNumber) {
    switch (mapperNumber) {
      case 0x000: return new Mapper000();
      case 0x001: return new Mapper001();
      default: throw std::exception("Mapper not implemented");
    }
  }

}