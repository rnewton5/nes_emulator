#include <nes/nes.h>

#include <iostream>

int main() {
  
  nes::Nes nes = nes::Nes("E:/emulator/nes/roms/nestest.nes");

  while (!nes.isStopped()) {
    nes.tick();
  }

  std::cin.get();
}