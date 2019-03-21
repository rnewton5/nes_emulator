#include <nes/cpu_bus.h>

#include <nes/cartridge/cartridge.h>
#include <nes/cpu.h>
#include <nes/ppu.h>

namespace nes {

  CpuBus::CpuBus(Ppu & ppu, Cartridge & cartridge) {
    this->ppu = &ppu;
    this->cartridge = &cartridge;
  }

  void CpuBus::write(WORD address, BYTE value) {
    if (address == 0x4014) {
      ppu->oamDma(value, ram);
      return;
    }

    if (address < 0x2000)
      ram[address % 0x800] = value;
    else if (address < 0x4000)
      ppu->write(address, value);
    else if (address < 0x4020)
      return; // TODO: reserved for apu and io
    else
      cartridge->write(address, value);
  }

  BYTE CpuBus::read(WORD address) {
    if (address < 0x2000)
      return ram[address % 0x800];
    if (address < 0x4000)
      return ppu->read(address);
    if (address < 0x4020)
      return 0; // TODO: reserved for apu and io
    return cartridge->read(address);
  }

  WORD CpuBus::read16(WORD address) {
    WORD lsb = read(address);
    WORD msb = read(address + 1);
    return (msb << 8) | lsb;
  }

}
