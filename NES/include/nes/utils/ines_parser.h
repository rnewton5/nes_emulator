#pragma once

#include <nes\common\globals.h>

#include <string>
#include <cstdint>
#include <vector>

namespace nes {

  class InesParser {
  public:
    InesParser(const std::string & fileName);

    std::vector<BYTE> getPrgRomData();
    std::vector<BYTE> getChrRomData();
    std::vector<BYTE> getPlayChoiceData();
    std::vector<BYTE> getTrainerData();

    int getMapperNumber();
    int getPrgRomSize();
    int getChrRomSize();
    bool isChrRam();
    int getMirroringMode();
    bool hasBatteryBackedRam();
    bool hasTrainer();
    bool hasVsUnisystem();
    bool hasPlayChoiceData();
    int getPrgRamSize();
    bool hasPrgRam();
    bool hasBusConflicts();

  private:
    std::vector<BYTE> fileData;
    bool isNes2Format;
  };

}