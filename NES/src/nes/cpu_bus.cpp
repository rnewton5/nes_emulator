#include <nes\cpu_bus.h>

#include <nes\cartridge\cartridge.h>
#include <nes\cpu.h>
#include <nes\ppu.h>

namespace nes {

  CpuBus::CpuBus(Ppu & ppu, Cartridge & cartridge) {
    this->ppu = &ppu;
    this->cartridge = &cartridge;
  }

  void CpuBus::write(WORD address, BYTE value) {
    if (address < 0x4000)
      this->ppu->write(address, value);
    else if (address < 0x4020)
      //reserved for apu and io
      return;
    else
      this->cartridge->write(address, value);
  }

  BYTE CpuBus::readByte(WORD address) {
    if (address < 0x4000) 
      return this->ppu->read(address);
    if (address < 0x4020)
      //reserved for apu and io
      return 0;
    return this->cartridge->read(address);
  }

  WORD CpuBus::readWord(WORD address) {
    WORD lsb = readByte(address);
    WORD msb = readByte(address + 1);
    return (msb << 8) | lsb;
  }

}
