#pragma once

#include <nes\common\globals.h>
#include <nes\interrupt_bus.h>
#include <nes\ppu_bus.h>
#include <nes\palette.h>
#include <nes\display.h>

namespace nes {



  class Ppu {
  public:
    Ppu(PpuBus & bus, Display & display, InterruptBus & interruptBus);

    // https://wiki.nesdev.com/w/index.php/PPU_power_up_state
    void reset();
    void init();

    void tick();

    void writeCtrl(BYTE value);
    void writeMask(BYTE value);
    void writeOamAddr(BYTE value);
    void writeOamData(BYTE value);
    void writeScroll(BYTE value);
    void writeAddr(BYTE value);
    void writeData(BYTE value);
    void writeLatch(BYTE value);

    void writeToOam(int index, BYTE value);

    BYTE readStatus();
    BYTE readOamData();
    BYTE readData();
    BYTE readLatch();

  private:
    InterruptBus * interruptBus;
    PpuBus * bus;
    Palette palette;

    BYTE oam[256];

    // The ppu exposes eight memory mapped registers to the Cpu's address space.
    //  they are mirrored every eight bytes from $2008 -> $3FFF
    BYTE regPpuCtrl; // $2000
    BYTE regPpuMask; // $2001
    BYTE regPpuStat; // $2002
    BYTE regOamAddr; // $2003
    BYTE regOamData; // $2004
    WORD regPpuScrl; // $2005
    WORD regPpuAddr; // $2006
    BYTE regPpuData; // $2007
    BYTE regOamDma;  // $4014

    BYTE latch;
    bool writeToggle; 
    int scanLineNum;
    int pixelNum;
    int frameNum;

    void render();
  };

}