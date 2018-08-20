#include <nes\ppu_bus.h>
#include <nes\cartridge\cartridge.h>

nes::PpuBus::PpuBus(Cartridge & cartridge) {
  this->cartridge = &cartridge;
}

void nes::PpuBus::write(WORD address, BYTE value) {
  if (address > 0x3F00)
    writeToPalette(address, value);
  else if (address > 0x2000)
    ram[(address - 0x2000) % 0x2FFF] = value;
  else
    cartridge->write(address, value);
}

BYTE nes::PpuBus::read(WORD address) {
  if (address > 0x3F00)
    return paletteIndexes[(address - 0x3F00) % 0x3F1F];
  if (address > 0x2000)
    return ram[(address - 0x2000) % 0x2FFF];
  else
    return cartridge->read(address);
}

WORD nes::PpuBus::read16(WORD address) {
  WORD lsb = read(address);
  WORD msb = read(address + 1);
  return (msb << 8) | lsb;
}

void nes::PpuBus::writeToPalette(WORD address, BYTE value) {
  int index = (address - 0x3F00) % 0x3F1F;
  if (index % 4 == 0) {
    for (int i = 0; i < 32; i += 4)
      paletteIndexes[i] = value;
  } else {
    paletteIndexes[index] = value;
  }
}
