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
    if (address == 0x4014) {
      ppu->writeLatch(value);
      for (int i = 0; i < 256; i++) {
        WORD oamAddress = ((WORD)value * 0x0100) + i;
        ppu->writeToOam(i, read(oamAddress));
      }
      return;
    }

    if (address < 0x2000)
      ram[address % 0x800] = value;
    else if (address < 0x4000)
      writeToPpu(address, value);
    else if (address < 0x4020)
      //reserved for apu and io
      return;
    else
      cartridge->write(address, value);
  }

  BYTE CpuBus::read(WORD address) {
    if (address < 0x2000)
      return ram[address % 0x800];
    if (address < 0x4000)
      return readFromPpu(address);
    if (address < 0x4020)
      //reserved for apu and io
      return 0;
    return cartridge->read(address);
  }

  WORD CpuBus::read16(WORD address) {
    WORD lsb = read(address);
    WORD msb = read(address + 1);
    return (msb << 8) | lsb;
  }

  BYTE CpuBus::readFromPpu(WORD address) {
    address = (address % 0x2008) + 0x2000;
    switch (address) {
      case 0x2002: return ppu->readStatus();
      case 0x2004: return ppu->readOamData();
      case 0x2007: return ppu->readData();
      default: return ppu->readLatch();
    }
  }

  void CpuBus::writeToPpu(WORD address, BYTE value) {
    address = (address % 0x2008) + 0x2000;
    switch (address) {
      case 0x2000: ppu->writeCtrl(value);    break;
      case 0x2001: ppu->writeMask(value);    break;
      case 0x2003: ppu->writeOamAddr(value); break;
      case 0x2004: ppu->writeOamData(value); break;
      case 0x2005: ppu->writeScroll(value);  break;
      case 0x2006: ppu->writeAddr(value);    break;
      case 0x2007: ppu->writeData(value);    break;
      default: ppu->writeLatch(value);
    }
  }

}
