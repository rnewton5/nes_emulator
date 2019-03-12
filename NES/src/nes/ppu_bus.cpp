#include <nes/ppu_bus.h>
#include <nes/cartridge/cartridge.h>

namespace nes {

  PpuBus::PpuBus(Cartridge & cartridge) {
    this->cartridge = &cartridge;
  }

  void PpuBus::write(WORD address, BYTE value) {
    if (address > 0x3F00) {
      writeToPalette(address, value);
    } else if (address > 0x2000) {
      int index = getRamIndex(address);
      ram[index] = value;
    } else {
      cartridge->write(address, value);
    }
  }

  BYTE PpuBus::read(WORD address) {
    if (address > 0x3F00) {
      int index = getPaletteIndex(address);
      return paletteIndexes[index];
    }

    if (address > 0x2000) {
      int index = getRamIndex(address);
      return ram[index];
    }
    
    return cartridge->read(address);
  }

  WORD PpuBus::read16(WORD address) {
    WORD lsb = read(address);
    WORD msb = read(address + 1);
    return (msb << 8) | lsb;
  }

  void PpuBus::writeToPalette(WORD address, BYTE value) {
    int index = getPaletteIndex(address);
    if (index % 4 == 0) {
      for (int i = 0; i < 32; i += 4)
        paletteIndexes[i] = value;
    } else {
      paletteIndexes[index] = value;
    }
  }

  int PpuBus::getPaletteIndex(WORD address) {
    return (address - 0x3F00) % (0x3F1F - 0x3F00);
  }

  int PpuBus::getRamIndex(WORD address) {
    return (address - 0x2000) % (0x2FFF - 0x2000);
  }
}
