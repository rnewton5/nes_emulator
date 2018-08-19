#include <nes\ppu_bus.h>
#include <nes\cartridge\cartridge.h>

nes::PpuBus::PpuBus(Cartridge & cartridge) {
  this->cartridge = &cartridge;
}

void nes::PpuBus::write(WORD address, BYTE value) {
  if (address > 0x3F00)
    paletteIndexes[(address - 0x3F00) % 0x3F1F] = value;
  else if (address > 0x2000)
    ram[(address - 0x2000) % 0x2FFF] = value;
  else
    cartridge->write(address, value);
}

BYTE nes::PpuBus::readByte(WORD address) {
  if (address > 0x3F00)
    return paletteIndexes[(address - 0x3F00) % 0x3F1F];
  if (address > 0x2000)
    return ram[(address - 0x2000) % 0x2FFF];
  else
    return cartridge->read(address);
}

WORD nes::PpuBus::readWord(WORD address) {
  WORD lsb = readByte(address);
  WORD msb = readByte(address + 1);
  return (msb << 8) | lsb;
}
