#include <nes/utils/ines_parser.h>

#include <fstream>
#include <iterator>

namespace nes {

  InesParser::InesParser(const std::string & fileName) {
    std::ifstream inesFile(fileName, std::ios::binary);
    inesFile.unsetf(std::ios::skipws);
    std::streampos fileSize;
    inesFile.seekg(0, std::ios::end);
    fileSize = inesFile.tellg();
    inesFile.seekg(0, std::ios::beg);

    fileData.reserve((int)fileSize);
    fileData.insert(fileData.begin(),
      std::istream_iterator<BYTE>(inesFile),
      std::istream_iterator<BYTE>());

    inesFile.close();

    // this is not the recommended detection procedure...
    isNes2Format = ((fileData[7] >> 2) & 0x3) == 2;
  }

  std::vector<BYTE> InesParser::getPrgRomData() {
    int prgRomOffset = hasTrainer() ? 528 : 16;
    std::vector<BYTE>::const_iterator begining =
      fileData.begin() + prgRomOffset;
    std::vector<BYTE>::const_iterator end =
      fileData.begin() + prgRomOffset + getPrgRomSize();

    return std::vector<BYTE>(begining, end);
  }

  std::vector<BYTE> InesParser::getChrRomData() {
    int chrRomOffset = getPrgRomSize();
    chrRomOffset += hasTrainer() ? 528 : 16;

    std::vector<BYTE>::const_iterator begining =
      fileData.begin() + chrRomOffset;
    std::vector<BYTE>::const_iterator end =
      fileData.begin() + chrRomOffset + getChrRomSize();

    return std::vector<BYTE>(begining, end);
  }

  std::vector<BYTE> InesParser::getPlayChoiceData() {
    // TODO: this method
    return std::vector<BYTE>();
  }

  std::vector<BYTE> InesParser::getTrainerData() {
    if (!hasTrainer()) {
      return std::vector<BYTE>();
    }
    std::vector<BYTE>::const_iterator begining =
      fileData.begin() + 16;
    std::vector<BYTE>::const_iterator end =
      fileData.begin() + 528;

    return std::vector<BYTE>(begining, end);
  }

  int InesParser::getMapperNumber() {
    if (isNes2Format) {
      return ((fileData[8] & 0xf0) << 4) | (fileData[7] & 0xf0) | (fileData[6] >> 4);
    }
    return (fileData[7] & 0xf0) | (fileData[6] >> 4);
  }

  int InesParser::getPrgRomSize() {
    return fileData[4] * 0x4000;
  }

  int InesParser::getChrRomSize() {
    return fileData[5] * 0x2000;
  }

  bool InesParser::isChrRam() {
    return fileData[5] == 0;
  }

  int InesParser::getMirroringMode() {
    if (fileData[6] & 0x8)
      return MIRROR_FOUR_SCREEN;
    else if (fileData[6] & 0x1)
      return MIRROR_VERTICAL;
    else
      return MIRROR_HORIZONTAL;
  }

  bool InesParser::hasBatteryBackedRam() {
    return fileData[6] & 0x2;
  }

  bool InesParser::hasTrainer() {
    return fileData[6] & 0x4;
  }

  bool InesParser::hasVsUnisystem() {
    //TODO: this method
    return false;
  }

  bool InesParser::hasPlayChoiceData() {
    //TODO: this method
    return false;
  }

  int InesParser::getPrgRamSize() {
    return fileData[8] ? fileData[8] * 0x2000 : 0x2000;
  }

  bool InesParser::hasPrgRam() {
    return fileData[10] & 0x10;
  }

  bool InesParser::hasBusConflicts() {
    return fileData[10] & 0x20;
  }

}