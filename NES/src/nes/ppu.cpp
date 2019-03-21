#include <nes/ppu.h>

namespace nes {

  Ppu::Ppu(Cartridge & cartridge, Display & display, InterruptBus & interruptBus) :
    bus(cartridge) {
    this->interruptBus = &interruptBus;
    this->palette = Palette();
    init();
  }

  void Ppu::init() {
    control = 0x00;
    mask = 0x00;
    status = 0xA0;
    regOamAddr = 0x00;
    regPpuScrl = 0x00;
    vramAddress = 0x00;
    latch = 0x00;
    writeToggle = true;
    scanLineNum = -1;
    cycles = 0;
  }

  void Ppu::reset() {
    control = 0x00;
    mask = 0x00;
    status &= 0x80;
    regPpuScrl = 0x00;
    latch = 0x00;
    writeToggle = true;
    scanLineNum = -1;
    cycles = 0;
  }

  BYTE Ppu::read(WORD address) {
    address = (address % 0x2008) + 0x2000;
    switch (address) {
      case 0x2002: return readStatus();
      case 0x2004: return readOamData();
      case 0x2007: return readData();
      default: return readLatch();
    }
  }

  void Ppu::write(WORD address, BYTE value) {
    address = (address % 0x2008) + 0x2000;
    switch (address) {
      case 0x2000: writeCtrl(value);    break;
      case 0x2001: writeMask(value);    break;
      case 0x2003: writeOamAddr(value); break;
      case 0x2004: writeOamData(value); break;
      case 0x2005: writeScroll(value);  break;
      case 0x2006: writeAddr(value);    break;
      case 0x2007: writeData(value);    break;
      default: writeLatch(value);
    }
  }

  void Ppu::oamDma(WORD startAddress, BYTE * cpuRam) {
    writeLatch(startAddress);
    WORD address = ((WORD)startAddress * 0x0100);
    for (int i = 0; i < 256; i++) {
      oam[i] = cpuRam[address + i];
    }
  }

  void Ppu::tick() {
    if (scanLineNum <= 239) {
      // mem access

      // render
      if (isVisibleScanLine() && isRendering()) {
        renderDot();
      }
    }

    // Update cycles and scanLineNum
    // TODO: add support for PAL systems
    cycles++;
    if (cycles > 340) {
      cycles = 0;
      scanLineNum++;
      if (scanLineNum > 260) {
        scanLineNum = -1;
      }
    }

    //set or reset VBlank flag
    if (scanLineNum == 241 && cycles == 1)
      setVBlank();
    else if (scanLineNum == -1 && cycles == 1)
      clearVBlank();
  }

  void Ppu::renderDot() {

  }

  void Ppu::writeCtrl(BYTE value) {
    control = value;
    latch = value;
  }

  void Ppu::writeMask(BYTE value) {
    mask = value;
    latch = value;
  }

  void Ppu::writeOamAddr(BYTE value) {
    regOamAddr = value;
    latch = regOamAddr;
  }

  void Ppu::writeOamData(BYTE value) {
    regOamData = value;
    oam[regOamAddr] = regOamData;
    latch = regOamData;
    regOamAddr += 1;
  }

  void Ppu::writeScroll(BYTE value) {
    if (!writeToggle)
      regPpuScrl = ((WORD)latch << 8) | value;
    writeToggle = !writeToggle;
    latch = value;
  }

  void Ppu::writeAddr(BYTE value) {
    if (!writeToggle)
      vramAddress = ((WORD)latch << 8) | value;
    writeToggle = !writeToggle;
    latch = value;
  }

  void Ppu::writeData(BYTE value) {
    writeBus(vramAddress, value);
    if ((control & 0x4) != 0)
      vramAddress += 32;
    else
      vramAddress += 1;
    latch = value;
  }

  void Ppu::writeLatch(BYTE value) {
    latch = value;
  }

  BYTE Ppu::readStatus() {
    writeToggle = true;
    latch = 0;
    return status;
  }

  BYTE Ppu::readOamData() {
    latch = regOamData;
    return regOamData;
  }

  BYTE Ppu::readData() {
    if ((control & 0x4) != 0)
      vramAddress += 32;
    else
      vramAddress += 1;
    latch = readBus(vramAddress);
    return latch;
  }

  BYTE Ppu::readLatch() {
    return latch;
  }


  bool Ppu::isRendering() {
    bool renderSprites = isRenderingSprites();
    bool renderBackground = isRenderingBackground();

    return (renderSprites || renderBackground);
  }

  bool Ppu::isRenderingSprites() {
    return (mask & 0x10);
  }

  bool Ppu::isRenderingBackground() {
    return (mask & 0x04);
  }

  bool Ppu::isVisibleScanLine() {
    return (scanLineNum >= 0 && scanLineNum <= 239);
  }

  void Ppu::setVBlank() {
    status |= 0x80;
  }

  void Ppu::clearVBlank() {
    status &= 0x7F;
  }

  BYTE Ppu::readBus(WORD address) {
    return bus.read(address);
  }

  WORD Ppu::readBus16(WORD address) {
    return bus.read16(address);
  }

  void Ppu::writeBus(WORD address, BYTE value) {
    bus.write(address, value);
  }
}