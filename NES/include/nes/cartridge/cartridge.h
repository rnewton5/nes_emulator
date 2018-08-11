#pragma once

#include <nes\cartridge\mappers\mapper.h>
#include <vector>
#include <nes\common\globals.h>

namespace nes {

  class Cartridge {
  public:
    Cartridge(std::string file);
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
    bool irqOccured();
    void clearIrq();

  private:
    Mapper * mapper;
    std::vector<BYTE> prgRom;
    std::vector<BYTE> prgRam;
    std::vector<BYTE> chrMemory;

    int mirroringMode;
    bool isChrMemoryRam;
    bool hasBattery;
    bool irq; // only set by certain mappers
  };

}