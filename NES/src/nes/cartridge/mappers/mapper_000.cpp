#include <nes/cartridge/mappers/mapper_000.h>
#include <nes/cartridge/cartridge.h>

namespace nes {

  BYTE Mapper000::read(WORD address, Cartridge * cartridge) {
    if (address > ROM_STARTING_ADDRESS)
      return cartridge->getPrgRom()[(address - ROM_STARTING_ADDRESS) % cartridge->getPrgRom().size()];
    if (address > RAM_STARTING_ADDRESS)
      return cartridge->getPrgRam()[address - RAM_STARTING_ADDRESS];
    if (address > CHR_STARTING_ADDRESS)
      return cartridge->getChrMem()[address - CHR_STARTING_ADDRESS];
    throw std::exception("Read from invalid cartidge space");
  }

  void Mapper000::write(WORD address, BYTE value, Cartridge * cartridge) {
    if (address > ROM_STARTING_ADDRESS)
      return;
    else if (address > RAM_STARTING_ADDRESS)
      cartridge->getPrgRam()[address - RAM_STARTING_ADDRESS] = value;
    else if (address > CHR_STARTING_ADDRESS && cartridge->hasChrRam())
      cartridge->getChrMem()[address - CHR_STARTING_ADDRESS] = value;
  }

}