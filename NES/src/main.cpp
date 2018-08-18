#include <nes\cartridge\cartridge.h>
#include <nes\cpu.h>
#include <nes\ppu.h>

#include <iostream>

int main() {

  nes::InterruptBus interruptBus = nes::InterruptBus();

  nes::Cartridge cartridge = nes::Cartridge("F:/everything else/emulator/nes/roms/nestest.nes", interruptBus);
  nes::Ppu ppu = nes::Ppu();

  nes::CpuBus cpuBus = nes::CpuBus(ppu, cartridge);
  nes::Cpu cpu = nes::Cpu(cpuBus, interruptBus);

  while (!cpu.isStopped()) {
    cpu.tick();
  }

  std::cin.get();
}