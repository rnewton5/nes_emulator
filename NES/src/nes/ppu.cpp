#include <nes/ppu.h>

namespace nes {

  Ppu::Ppu(PpuBus & bus, Display & display ,InterruptBus & interruptBus) {
    this->interruptBus = &interruptBus;
    this->bus = &bus;
    this->palette = Palette();
    init();
  }

  // for info on the ppu state after startup and reset, go to
  //   https://wiki.nesdev.com/w/index.php/PPU_power_up_state
  void Ppu::init() {
    regPpuCtrl = 0x00;   // 0000 0000
    regPpuMask = 0x00;   // 0000 0000
    regPpuStat = 0xA0;   // +0+x xxxx
    regOamAddr = 0x00;   // $00
    regPpuScrl = 0x00;   // $0000
    regPpuAddr = 0x00;   // $0000
    regPpuData = 0x00;   // $00
    latch = 0x00;        // latch cleared
    writeToggle = true;
    scanLineNum = 261;
    pixelNum = 0;
  }

  void Ppu::reset() {
    regPpuCtrl = 0x00;              // 0000 0000
    regPpuMask = 0x00;              // 0000 0000
    regPpuStat = regPpuStat & 0x80; // U??x xxxx
    regOamAddr = regOamAddr;        // unchanged
    regPpuScrl = 0x00;              // $0000
    regPpuAddr = regPpuAddr;        // unchanged
    regPpuData = 0x00;              // $00
    latch = 0x00;                   // latch cleared
    writeToggle = true;
    scanLineNum = 261;
    pixelNum = 0;
  }

  void Ppu::tick() {
    render();

    // TODO: these values are for NTSC systems. Ideally, we should
    //  check the region of the game and use the appropriate values.
    scanLineNum++;
    scanLineNum %= 262;

    pixelNum++;
    pixelNum %= 341;
  }

  void Ppu::writeCtrl(BYTE value) {
    regPpuCtrl = value;
    latch = value;
  }

  void Ppu::writeMask(BYTE value) {
    regPpuMask = value;
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
      regPpuAddr = ((WORD)latch << 8) | value;
    writeToggle = !writeToggle;
    latch = value;
  }

  void Ppu::writeData(BYTE value) {
    bus->write(regPpuAddr, value);
    if ((regPpuCtrl & 0x4) != 0)
      regPpuAddr += 32;
    else
      regPpuAddr += 1;
    regPpuData = value;
    latch = regPpuData;
  }

  void Ppu::writeLatch(BYTE value) {
    latch = value;
  }

  BYTE Ppu::readStatus() {
    writeToggle = true;
    latch = 0;
    return regPpuStat;
  }

  BYTE Ppu::readOamData() {
    latch = regOamData;
    return regOamData;
  }

  BYTE Ppu::readData() {
    regPpuData = bus->read(regPpuAddr);
    if ((regPpuCtrl & 0x4) != 0)
      regPpuAddr += 32;
    else
      regPpuAddr += 1;
    latch = regPpuData;
    return regPpuData;
  }

  BYTE Ppu::readLatch() {
    return latch;
  }

  void Ppu::writeToOam(int index, BYTE value) {
    oam[index] = value;
  }

  void Ppu::render() {
    
  }
}