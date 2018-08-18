#include <nes\cartridge\cartridge.h>
#include <nes\utils\ines_parser.h>
#include <nes\cartridge\mappers\mapper_factory.h>
#include <string>

namespace nes {

  Cartridge::Cartridge(std::string file, InterruptBus & interruptBus) {
    InesParser inp = InesParser(file);

    if (inp.isChrRam()) {
      isChrMemoryRam = true;
      chrMemory = std::vector<BYTE>(0x2000);
    } else {
      chrMemory = inp.getChrRomData();
    }

    prgRom = inp.getPrgRomData();
    prgRam = std::vector<BYTE>(inp.getPrgRamSize());
    mirroringMode = inp.getMirroringMode();
    hasBattery = inp.hasBatteryBackedRam();
    mapper = MapperFactory::Create(inp.getMapperNumber());
    this->interruptBus = &interruptBus;
  }

  Cartridge::Cartridge(const Cartridge & cart) {
    prgRom = std::vector<BYTE>(cart.prgRom);
    prgRam = std::vector<BYTE>(cart.prgRam);
    chrMemory = std::vector<BYTE>(cart.chrMemory);
    mirroringMode = cart.mirroringMode;
    isChrMemoryRam = cart.isChrMemoryRam;
    hasBattery = cart.hasBattery;
    mapper = cart.mapper;
  }

  Cartridge::~Cartridge() {
    delete mapper;
  }

  BYTE Cartridge::read(WORD address) {
    return mapper->read(address, this);
  }

  void Cartridge::write(WORD address, BYTE value) {
    mapper->write(address, value, this);
  }

  std::vector<BYTE> & Cartridge::getPrgRom() {
    return prgRom;
  }

  std::vector<BYTE> & Cartridge::getPrgRam() {
    return prgRam;
  }

  std::vector<BYTE> & Cartridge::getChrMem() {
    return chrMemory;
  }

  int Cartridge::getMirroringMode() {
    return mirroringMode;
  }

  void Cartridge::setMirroringMode(int mirroringMode) {
    this->mirroringMode = mirroringMode;
  }

  bool Cartridge::hasChrRam() {
    return isChrMemoryRam;
  }

  bool Cartridge::hasBatteryBackedRam() {
    return hasBattery;
  }
}