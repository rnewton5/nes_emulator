#pragma once

#include <nes/common/globals.h>
#include <nes/interrupt_bus.h>
#include <nes/ppu_bus.h>
#include <nes/palette.h>
#include <nes/display.h>

namespace nes {



  class Ppu {
  public:
    Ppu(Cartridge & cartridge, Display & display, InterruptBus & interruptBus);

    void init();
    void reset();

    BYTE read(WORD address);
    void write(WORD address, BYTE value);
    void tick();

    void oamDma(WORD startAddress, BYTE * cpuRam);

  private:
    InterruptBus * interruptBus;
    PpuBus bus;
    Palette palette;

    BYTE oam[256];

    int scanLineNum;
    int cycles;

    BYTE control; 
    BYTE mask; 
    BYTE status;
    BYTE regOamAddr; 
    BYTE regOamData; 
    WORD regPpuScrl; 

    BYTE latch;
    bool oddFrameFlag;
    WORD vramAddress;
    WORD tempVramAddress;
    bool writeToggle; 

    void renderDot();

    void writeCtrl(BYTE value);    // $2000
    void writeMask(BYTE value);    // $2001
    void writeOamAddr(BYTE value); // $2003
    void writeOamData(BYTE value); // $2004
    void writeScroll(BYTE value);  // $2005
    void writeAddr(BYTE value);    // $2006
    void writeData(BYTE value);    // $2007
    void writeLatch(BYTE value);

    BYTE readStatus();  // $2002
    BYTE readOamData(); // $2004
    BYTE readData();    // $2007
    BYTE readLatch();

    bool isRendering();
    bool isRenderingSprites();
    bool isRenderingBackground();
    bool isVisibleScanLine();
    void setVBlank();
    void clearVBlank();

    BYTE readBus(WORD address);
    WORD readBus16(WORD address);
    void writeBus(WORD address, BYTE value);
  };

}