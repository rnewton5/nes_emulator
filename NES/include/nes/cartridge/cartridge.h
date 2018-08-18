#pragma once

#include <nes\common\globals.h>
#include <nes\cartridge\mappers\mapper.h>
#include <nes\interrupt_bus.h>

#include <vector>

namespace nes {

  class Cartridge {
  public:
    Cartridge(std::string file, InterruptBus & interruptBus);
    Cartridge(const Cartridge & cart);
    ~Cartridge();

    BYTE read(WORD address);
    void write(WORD address, BYTE value);

    std::vector<BYTE> & getPrgRom();
    std::vector<BYTE> & getPrgRam();
    std::vector<BYTE> & getChrMem();

    int getMirroringMode();
    void setMirroringMode(int mirroringMode);
    bool hasChrRam();
    bool hasBatteryBackedRam();

  private:
    InterruptBus * interruptBus;
    Mapper * mapper;
    std::vector<BYTE> prgRom;
    std::vector<BYTE> prgRam;
    std::vector<BYTE> chrMemory;

    int mirroringMode;
    bool isChrMemoryRam;
    bool hasBattery;
  };

}