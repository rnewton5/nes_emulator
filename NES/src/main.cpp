#include <nes\cartridge\cartridge.h>
#include <nes\cpu.h>

#include <iostream>

int main() {
  nes::Cartridge cartridge = nes::Cartridge("F:/everything else/emulator/nes/roms/nestest.nes");
  nes::Cpu cpu = nes::Cpu(cartridge);

  while (!cpu.isStopped()) {
    cpu.executeNextInstruction();
  }

  std::cin.get();
}