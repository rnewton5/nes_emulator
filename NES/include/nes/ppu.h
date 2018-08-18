#pragma once

#include <nes\common\globals.h>
#include <nes\interrupt_bus.h>
#include <nes\palette.h>

namespace nes {



  class Ppu {
  public:
    Ppu(InterruptBus & interruptBus);

    // https://wiki.nesdev.com/w/index.php/PPU_power_up_state
    void reset();
    void init();

    void write(WORD address, BYTE value);
    BYTE read(WORD address);

  private:
    InterruptBus * interruptBus;
    Palette palette;

    // The ppu exposes eight memory mapped registers to the Cpu's address space.
    //  they are mirrored every eight bytes from $2008 -> $3FFF
    BYTE ppuCtrl;   // $2000
    BYTE ppuMask;   // $2001
    BYTE ppuStatus; // $2002
    BYTE oamAddr;   // $2003
    BYTE oamData;   // $2004
    WORD ppuScroll; // $2005
    WORD ppuAddr;   // $2006
    BYTE ppuData;   // $2007

    BYTE oamDma;    // $4014

    BYTE latch;
    bool writeToggle; 
  };

}