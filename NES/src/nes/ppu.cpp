#include <nes/ppu.h>

namespace nes {

  Ppu::Ppu(InterruptBus & interruptBus) {
    this->interruptBus = &interruptBus;
    this->palette = Palette();
    init();
  }

  void Ppu::init() {
    ppuCtrl   = 0x00; // 0000 0000
    ppuMask   = 0x00; // 0000 0000
    ppuStatus = 0xA0; // +0+x xxxx
    oamAddr   = 0x00; // $00
    latch     = 0x00; // latch cleared
    ppuScroll = 0x00; // $0000
    ppuAddr   = 0x00; // $0000
    ppuData   = 0x00; // $00
    writeToggle = true;
  }

  void Ppu::reset() {
    ppuCtrl   = 0x00;             // 0000 0000
    ppuMask   = 0x00;             // 0000 0000
    ppuStatus = ppuStatus & 0x80; // U??x xxxx
    oamAddr   = oamAddr;          // unchanged
    latch     = 0x00;             // latch cleared
    ppuScroll = 0x00;             // $0000
    ppuAddr   = ppuAddr;          // unchanged
    ppuData   = 0x00;             // $00
    writeToggle = true;
  }

  void Ppu::write(WORD address, BYTE value) {
    if (address == 0x4014) {
      oamDma = value;
    }

    address = (address % 0x2008) + 0x2000;
    switch (address) {
      case 0x2000: ppuCtrl = value; break;
      case 0x2001: ppuMask = value; break;
      case 0x2003: oamAddr = value; break;
      case 0x2004: oamData = value; break;
      case 0x2005: {
        if (!writeToggle)
          ppuScroll = ((WORD)latch << 8) | value;
        writeToggle = !writeToggle;
        break;
      }
      case 0x2006: {
        if (!writeToggle)
          ppuAddr = ((WORD)latch << 8) | value;
        writeToggle = !writeToggle;
        break;
      }
      case 0x2007: ppuData = value; break;
    }
    latch = value;
  }

  BYTE Ppu::read(WORD address) {
    if (address == 0x4014) {
      latch = oamDma;
    }

    address = (address % 0x2008) + 0x2000;
    switch (address) {
      case 0x2002: {
        latch = 0;
        return ppuStatus;
      }
      case 0x2004: {
        latch = oamData;
        return oamData;
      }
      case 0x2007: {
        latch = ppuData;
        return ppuData;
      }
      default: {
        return latch;
      }
    }
  }
}