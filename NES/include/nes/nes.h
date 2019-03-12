#pragma once

#include <nes/cartridge/cartridge.h>
#include <nes/cpu.h>
#include <nes/ppu.h>

#include <string>

namespace nes {

  class Nes {
  public:
    Nes(std::string file);
    bool isStopped();
    void tick();

  private:
    Display display;
    InterruptBus interruptBus;
    Cartridge cartridge;
    Ppu ppu;
    Cpu cpu;
  };

}