#pragma once

#include <nes\common\globals.h>
#include <nes\interrupt_bus.h>
#include <nes\ppu_bus.h>
#include <nes\palette.h>

namespace nes {



  class Ppu {
  public:
    Ppu(PpuBus & bus, InterruptBus & interruptBus);

    // https://wiki.nesdev.com/w/index.php/PPU_power_up_state
    void reset();
    void init();

    void write(WORD address, BYTE value);
    BYTE read(WORD address);

    void writeToOam(int index, BYTE value);

  private:
    InterruptBus * interruptBus;
    PpuBus * bus;
    Palette palette;

    BYTE oam[256];

    // The ppu exposes eight memory mapped registers to the Cpu's address space.
    //  they are mirrored every eight bytes from $2008 -> $3FFF
    BYTE regPpuCtrl;   // $2000
    BYTE regPpuMask;   // $2001
    BYTE regPpuStatus; // $2002
    BYTE regOamAddr;   // $2003
    BYTE regOamData;   // $2004
    WORD regPpuScroll; // $2005
    WORD regPpuAddr;   // $2006
    BYTE regPpuData;   // $2007
    BYTE regOamDma;    // $4014

    BYTE latch;
    bool writeToggle; 
  };

}