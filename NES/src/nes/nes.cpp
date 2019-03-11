#include <nes/nes.h>

namespace nes {

  Nes::Nes(std::string file) :
    cartridge(file, interruptBus),
    ppuBus(cartridge),
    ppu(ppuBus, display, interruptBus),
    cpuBus(ppu, cartridge),
    cpu(cpuBus, interruptBus)
  { }

  void Nes::tick() {
    // ppu ticks 3 times for every cpu cycle
    ppu.tick();
    ppu.tick();
    ppu.tick();

    cpu.tick();
  }

  bool Nes::isStopped() {
    return cpu.isStopped();
  }
}