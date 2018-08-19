#include <nes/ppu.h>

namespace nes {

  Ppu::Ppu(PpuBus & bus, InterruptBus & interruptBus) {
    this->interruptBus = &interruptBus;
    this->bus = &bus;
    this->palette = Palette();
    init();
  }

  void Ppu::init() {
    regPpuCtrl     = 0x00; // 0000 0000
    regPpuMask     = 0x00; // 0000 0000
    regPpuStatus   = 0xA0; // +0+x xxxx
    regOamAddr     = 0x00; // $00
    latch       = 0x00; // latch cleared
    regPpuScroll   = 0x00; // $0000
    regPpuAddr     = 0x00; // $0000
    regPpuData     = 0x00; // $00
    writeToggle = true;
  }

  void Ppu::reset() {
    regPpuCtrl     = 0x00;             // 0000 0000
    regPpuMask     = 0x00;             // 0000 0000
    regPpuStatus   = regPpuStatus & 0x80; // U??x xxxx
    regOamAddr     = regOamAddr;          // unchanged
    latch       = 0x00;             // latch cleared
    regPpuScroll   = 0x00;             // $0000
    regPpuAddr     = regPpuAddr;          // unchanged
    regPpuData     = 0x00;             // $00
    writeToggle = true;
  }

  void Ppu::write(WORD address, BYTE value) {
    if (address == 0x4014) {
      regOamDma = value;
    }

    address = (address % 0x2008) + 0x2000;
    switch (address) {
      case 0x2000: regPpuCtrl = value; break;
      case 0x2001: regPpuMask = value; break;
      case 0x2003: regOamAddr = value; break;
      case 0x2004: regOamData = value; break;
      case 0x2005: {
        if (!writeToggle)
          regPpuScroll = ((WORD)latch << 8) | value;
        writeToggle = !writeToggle;
        break;
      }
      case 0x2006: {
        if (!writeToggle)
          regPpuAddr = ((WORD)latch << 8) | value;
        writeToggle = !writeToggle;
        break;
      }
      case 0x2007: regPpuData = value; break;
    }
    latch = value;
  }

  BYTE Ppu::read(WORD address) {
    if (address == 0x4014) {
      latch = regOamDma;
    }

    address = (address % 0x2008) + 0x2000;
    switch (address) {
      case 0x2002: {
        latch = 0;
        return regPpuStatus;
      }
      case 0x2004: {
        latch = regOamData;
        return regOamData;
      }
      case 0x2007: {
        latch = regPpuData;
        return regPpuData;
      }
      default: {
        return latch;
      }
    }
  }
  void Ppu::writeToOam(int index, BYTE value) {
    oam[index] = value;
  }
}