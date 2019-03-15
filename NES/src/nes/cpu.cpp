#include <nes/cpu.h>

namespace nes {

#define STACK_UPPER_BOUND      0x01FD
#define STACK_LOWER_BOUND      0x0100
#define CARRY_FLAG             0x01
#define ZERO_FLAG              0X02
#define INTERRUPT_DISABLE_FLAG 0X04
#define DECIMAL_MODE_FLAG      0X08
#define BREAK_FLAG             0X10
#define OVERFLOW_FLAG          0x40
#define NEGATIVE_FLAG          0x80

#define NESTEST_START 0
#define LOGGING 0

#if LOGGING
#include <stdio.h>
#endif

  //-----------------------------------------
  // public methods
  //-----------------------------------------
  Cpu::Cpu(Cartridge & cartridge, Ppu & ppu, InterruptBus & interruptBus) :
    bus(ppu, cartridge)
  {
    this->interruptBus = &interruptBus;
    reset();
  }

  void Cpu::reset() {
    regSP = STACK_UPPER_BOUND;
    regP = 0x24;
#if NESTEST_START 
    regPC = 0xC000;
#else 
    regPC = readBus16(0xFFFC);
#endif
    stopped = false;
    ppuCycles = 0;
  }

  bool Cpu::isStopped() {
    return stopped;
  }

  void Cpu::tick() {
    if (cycles > 0) {
      cycles--;
      ppuCycles += 3;
      ppuCycles %= 341;
    }
    else {
      executeNextInstruction();
    }
  }

  //-----------------------------------------
  // reading and writing
  //-----------------------------------------
  BYTE Cpu::readBus(WORD address) {
    return bus.read(address);
  }

  WORD Cpu::readBus16(WORD address) {
    return bus.read16(address);
  }

  void Cpu::writeBus(WORD address, BYTE value) {
    // TODO: check cycles
    if (address == 0x4014)
      cycles += 512;
    bus.write(address, value);
  }

  //-----------------------------------------
  // Gets and executes the next instruction
  //-----------------------------------------
  void Cpu::executeNextInstruction() {
    if (stopped)
      return;

    BYTE opcode = readBus(regPC++);
    pageCrossed = false;

    switch (opcode) {
      case 0x00: BRK(addressingMode::implied);     break;
      case 0x01: ORA(addressingMode::indirectX);   break;
      case 0x02: STP(addressingMode::implied);     break;
      case 0x03: SLO(addressingMode::indirectX);   break;
      case 0x04: NOP(addressingMode::zeroPage);    break;
      case 0x05: ORA(addressingMode::zeroPage);    break;
      case 0x06: ASL(addressingMode::zeroPage);    break;
      case 0x07: SLO(addressingMode::zeroPage);    break;
      case 0x08: PHP(addressingMode::implied);     break;
      case 0x09: ORA(addressingMode::immediate);   break;
      case 0x0A: ASL(addressingMode::implied);     break;
      case 0x0B: ANC(addressingMode::immediate);   break;
      case 0x0C: NOP(addressingMode::absolute);    break;
      case 0x0D: ORA(addressingMode::absolute);    break;
      case 0x0E: ASL(addressingMode::absolute);    break;
      case 0x0F: SLO(addressingMode::absolute);    break;
      case 0x10: BPL(addressingMode::immediate);   break;
      case 0x11: ORA(addressingMode::indirectY);   break;
      case 0x12: STP(addressingMode::implied);     break;
      case 0x13: SLO(addressingMode::indirectY);   break;
      case 0x14: NOP(addressingMode::zeroPageX);   break;
      case 0x15: ORA(addressingMode::zeroPageX);   break;
      case 0x16: ASL(addressingMode::zeroPageX);   break;
      case 0x17: SLO(addressingMode::zeroPageX);   break;
      case 0x18: CLC(addressingMode::implied);     break;
      case 0x19: ORA(addressingMode::absoluteY);   break;
      case 0x1A: NOP(addressingMode::implied);     break;
      case 0x1B: SLO(addressingMode::absoluteY);   break;
      case 0x1C: NOP(addressingMode::absoluteX);   break;
      case 0x1D: ORA(addressingMode::absoluteX);   break;
      case 0x1E: ASL(addressingMode::absoluteX);   break;
      case 0x1F: SLO(addressingMode::absoluteX);   break;
      case 0x20: JSR(addressingMode::absolute);    break;
      case 0x21: AND(addressingMode::indirectX);   break;
      case 0x22: STP(addressingMode::implied);     break;
      case 0x23: RLA(addressingMode::indirectX);   break;
      case 0x24: BIT(addressingMode::zeroPage);    break;
      case 0x25: AND(addressingMode::zeroPage);    break;
      case 0x26: ROL(addressingMode::zeroPage);    break;
      case 0x27: RLA(addressingMode::zeroPage);    break;
      case 0x28: PLP(addressingMode::implied);     break;
      case 0x29: AND(addressingMode::immediate);   break;
      case 0x2A: ROL(addressingMode::implied);     break;
      case 0x2B: ANC(addressingMode::immediate);   break;
      case 0x2C: BIT(addressingMode::absolute);    break;
      case 0x2D: AND(addressingMode::absolute);    break;
      case 0x2E: ROL(addressingMode::absolute);    break;
      case 0x2F: RLA(addressingMode::absolute);    break;
      case 0x30: BMI(addressingMode::immediate);   break;
      case 0x31: AND(addressingMode::indirectY);   break;
      case 0x32: STP(addressingMode::implied);     break;
      case 0x33: RLA(addressingMode::indirectY);   break;
      case 0x34: NOP(addressingMode::zeroPageX);   break;
      case 0x35: AND(addressingMode::zeroPageX);   break;
      case 0x36: ROL(addressingMode::zeroPageX);   break;
      case 0x37: RLA(addressingMode::zeroPageX);   break;
      case 0x38: SEC(addressingMode::implied);     break;
      case 0x39: AND(addressingMode::absoluteY);   break;
      case 0x3A: NOP(addressingMode::implied);     break;
      case 0x3B: RLA(addressingMode::absoluteY);   break;
      case 0x3C: NOP(addressingMode::absoluteX);   break;
      case 0x3D: AND(addressingMode::absoluteX);   break;
      case 0x3E: ROL(addressingMode::absoluteX);   break;
      case 0x3F: RLA(addressingMode::absoluteX);   break;
      case 0x40: RTI(addressingMode::implied);     break;
      case 0x41: EOR(addressingMode::indirectX);   break;
      case 0x42: STP(addressingMode::implied);     break;
      case 0x43: SRE(addressingMode::indirectX);   break;
      case 0x44: NOP(addressingMode::zeroPage);    break;
      case 0x45: EOR(addressingMode::zeroPage);    break;
      case 0x46: LSR(addressingMode::zeroPage);    break;
      case 0x47: SRE(addressingMode::zeroPage);    break;
      case 0x48: PHA(addressingMode::implied);     break;
      case 0x49: EOR(addressingMode::immediate);   break;
      case 0x4A: LSR(addressingMode::implied);     break;
      case 0x4B: ALR(addressingMode::immediate);   break;
      case 0x4C: JMP(addressingMode::absolute);    break;
      case 0x4D: EOR(addressingMode::absolute);    break;
      case 0x4E: LSR(addressingMode::absolute);    break;
      case 0x4F: SRE(addressingMode::absolute);    break;
      case 0x50: BVC(addressingMode::immediate);   break;
      case 0x51: EOR(addressingMode::indirectY);   break;
      case 0x52: STP(addressingMode::implied);     break;
      case 0x53: SRE(addressingMode::indirectY);   break;
      case 0x54: NOP(addressingMode::zeroPageX);   break;
      case 0x55: EOR(addressingMode::zeroPageX);   break;
      case 0x56: LSR(addressingMode::zeroPageX);   break;
      case 0x57: SRE(addressingMode::zeroPageX);   break;
      case 0x58: CLI(addressingMode::implied);     break;
      case 0x59: EOR(addressingMode::absoluteY);   break;
      case 0x5A: NOP(addressingMode::implied);     break;
      case 0x5B: SRE(addressingMode::absoluteY);   break;
      case 0x5C: NOP(addressingMode::absoluteX);   break;
      case 0x5D: EOR(addressingMode::absoluteX);   break;
      case 0x5E: LSR(addressingMode::absoluteX);   break;
      case 0x5F: SRE(addressingMode::absoluteX);   break;
      case 0x60: RTS(addressingMode::implied);     break;
      case 0x61: ADC(addressingMode::indirectX);   break;
      case 0x62: STP(addressingMode::implied);     break;
      case 0x63: RRA(addressingMode::indirectX);   break;
      case 0x64: NOP(addressingMode::zeroPage);    break;
      case 0x65: ADC(addressingMode::zeroPage);    break;
      case 0x66: ROR(addressingMode::zeroPage);    break;
      case 0x67: RRA(addressingMode::zeroPage);    break;
      case 0x68: PLA(addressingMode::implied);     break;
      case 0x69: ADC(addressingMode::immediate);   break;
      case 0x6A: ROR(addressingMode::implied);     break;
      case 0x6B: ARR(addressingMode::immediate);   break;
      case 0x6C: JMP(addressingMode::indirect);    break;
      case 0x6D: ADC(addressingMode::absolute);    break;
      case 0x6E: ROR(addressingMode::absolute);    break;
      case 0x6F: RRA(addressingMode::absolute);    break;
      case 0x70: BVS(addressingMode::immediate);   break;
      case 0x71: ADC(addressingMode::indirectY);   break;
      case 0x72: STP(addressingMode::implied);     break;
      case 0x73: RRA(addressingMode::indirectY);   break;
      case 0x74: NOP(addressingMode::zeroPageX);   break;
      case 0x75: ADC(addressingMode::zeroPageX);   break;
      case 0x76: ROR(addressingMode::zeroPageX);   break;
      case 0x77: RRA(addressingMode::zeroPageX);   break;
      case 0x78: SEI(addressingMode::implied);     break;
      case 0x79: ADC(addressingMode::absoluteY);   break;
      case 0x7A: NOP(addressingMode::implied);     break;
      case 0x7B: RRA(addressingMode::absoluteY);   break;
      case 0x7C: NOP(addressingMode::absoluteX);   break;
      case 0x7D: ADC(addressingMode::absoluteX);   break;
      case 0x7E: ROR(addressingMode::absoluteX);   break;
      case 0x7F: RRA(addressingMode::absoluteX);   break;
      case 0x80: NOP(addressingMode::immediate);   break;
      case 0x81: STA(addressingMode::indirectX);   break;
      case 0x82: NOP(addressingMode::immediate);   break;
      case 0x83: SAX(addressingMode::indirectX);   break;
      case 0x84: STY(addressingMode::zeroPage);    break;
      case 0x85: STA(addressingMode::zeroPage);    break;
      case 0x86: STX(addressingMode::zeroPage);    break;
      case 0x87: SAX(addressingMode::zeroPage);    break;
      case 0x88: DEY(addressingMode::implied);     break;
      case 0x89: NOP(addressingMode::immediate);   break;
      case 0x8A: TXA(addressingMode::implied);     break;
      case 0x8B: XAA(addressingMode::immediate);   break;
      case 0X8C: STY(addressingMode::absolute);    break;
      case 0x8D: STA(addressingMode::absolute);    break;
      case 0x8E: STX(addressingMode::absolute);    break;
      case 0x8F: SAX(addressingMode::absolute);    break;
      case 0x90: BCC(addressingMode::immediate);   break;
      case 0x91: STA(addressingMode::indirectY);   break;
      case 0x92: STP(addressingMode::implied);     break;
      case 0x93: AHX(addressingMode::indirectY);   break;
      case 0x94: STY(addressingMode::zeroPageX);   break;
      case 0x95: STA(addressingMode::zeroPageX);   break;
      case 0x96: STX(addressingMode::zeroPageY);   break;
      case 0x97: SAX(addressingMode::zeroPageY);   break;
      case 0x98: TYA(addressingMode::implied);     break;
      case 0x99: STA(addressingMode::absoluteY);   break;
      case 0x9A: TXS(addressingMode::implied);     break;
      case 0x9B: TAS(addressingMode::absoluteY);   break;
      case 0x9C: SHY(addressingMode::absoluteX);   break;
      case 0x9D: STA(addressingMode::absoluteX);   break;
      case 0x9E: SHX(addressingMode::absoluteY);   break;
      case 0x9F: AHX(addressingMode::absoluteY);   break;
      case 0xA0: LDY(addressingMode::immediate);   break;
      case 0xA1: LDA(addressingMode::indirectX);   break;
      case 0xA2: LDX(addressingMode::immediate);   break;
      case 0xA3: LAX(addressingMode::indirectX);   break;
      case 0xA4: LDY(addressingMode::zeroPage);    break;
      case 0xA5: LDA(addressingMode::zeroPage);    break;
      case 0xA6: LDX(addressingMode::zeroPage);    break;
      case 0xA7: LAX(addressingMode::zeroPage);    break;
      case 0xA8: TAY(addressingMode::implied);     break;
      case 0xA9: LDA(addressingMode::immediate);   break;
      case 0xAA: TAX(addressingMode::implied);     break;
      case 0xAB: LAX(addressingMode::immediate);   break;
      case 0xAC: LDY(addressingMode::absolute);    break;
      case 0xAD: LDA(addressingMode::absolute);    break;
      case 0xAE: LDX(addressingMode::absolute);    break;
      case 0xAF: LAX(addressingMode::absolute);    break;
      case 0xB0: BCS(addressingMode::immediate);   break;
      case 0xB1: LDA(addressingMode::indirectY);   break;
      case 0xB2: STP(addressingMode::implied);     break;
      case 0xB3: LAX(addressingMode::indirectY);   break;
      case 0xB4: LDY(addressingMode::zeroPageX);   break;
      case 0xB5: LDA(addressingMode::zeroPageX);   break;
      case 0xB6: LDX(addressingMode::zeroPageY);   break;
      case 0xB7: LAX(addressingMode::zeroPageY);   break;
      case 0xB8: CLV(addressingMode::implied);     break;
      case 0xB9: LDA(addressingMode::absoluteY);   break;
      case 0xBA: TSX(addressingMode::implied);     break;
      case 0xBB: LAS(addressingMode::absoluteY);   break;
      case 0xBC: LDY(addressingMode::absoluteX);   break;
      case 0xBD: LDA(addressingMode::absoluteX);   break;
      case 0xBE: LDX(addressingMode::absoluteY);   break;
      case 0xBF: LAX(addressingMode::absoluteY);   break;
      case 0xC0: CPY(addressingMode::immediate);   break;
      case 0xC1: CMP(addressingMode::indirectX);   break;
      case 0xC2: NOP(addressingMode::immediate);   break;
      case 0xC3: DCP(addressingMode::indirectX);   break;
      case 0xC4: CPY(addressingMode::zeroPage);    break;
      case 0xC5: CMP(addressingMode::zeroPage);    break;
      case 0xC6: DEC(addressingMode::zeroPage);    break;
      case 0xC7: DCP(addressingMode::zeroPage);    break;
      case 0xC8: INY(addressingMode::implied);     break;
      case 0xC9: CMP(addressingMode::immediate);   break;
      case 0xCA: DEX(addressingMode::implied);     break;
      case 0xCB: AXS(addressingMode::immediate);   break;
      case 0xCC: CPY(addressingMode::absolute);    break;
      case 0xCD: CMP(addressingMode::absolute);    break;
      case 0xCE: DEC(addressingMode::absolute);    break;
      case 0xCF: DCP(addressingMode::absolute);    break;
      case 0xD0: BNE(addressingMode::immediate);   break;
      case 0xD1: CMP(addressingMode::indirectY);   break;
      case 0xD2: STP(addressingMode::implied);     break;
      case 0xD3: DCP(addressingMode::indirectY);   break;
      case 0xD4: NOP(addressingMode::zeroPageX);   break;
      case 0xD5: CMP(addressingMode::zeroPageX);   break;
      case 0xD6: DEC(addressingMode::zeroPageX);   break;
      case 0xD7: DCP(addressingMode::zeroPageX);   break;
      case 0xD8: CLD(addressingMode::implied);     break;
      case 0xD9: CMP(addressingMode::absoluteY);   break;
      case 0xDA: NOP(addressingMode::implied);     break;
      case 0xDB: DCP(addressingMode::absoluteY);   break;
      case 0xDC: NOP(addressingMode::absoluteX);   break;
      case 0xDD: CMP(addressingMode::absoluteX);   break;
      case 0xDE: DEC(addressingMode::absoluteX);   break;
      case 0xDF: DCP(addressingMode::absoluteX);   break;
      case 0xE0: CPX(addressingMode::immediate);   break;
      case 0xE1: SBC(addressingMode::indirectX);   break;
      case 0xE2: NOP(addressingMode::immediate);   break;
      case 0xE3: ISC(addressingMode::indirectX);   break;
      case 0xE4: CPX(addressingMode::zeroPage);    break;
      case 0xE5: SBC(addressingMode::zeroPage);    break;
      case 0xE6: INC(addressingMode::zeroPage);    break;
      case 0xE7: ISC(addressingMode::zeroPage);    break;
      case 0xE8: INX(addressingMode::implied);     break;
      case 0xE9: SBC(addressingMode::immediate);   break;
      case 0xEA: NOP(addressingMode::implied);     break;
      case 0xEB: SBC(addressingMode::immediate);   break;
      case 0xEC: CPX(addressingMode::absolute);    break;
      case 0xED: SBC(addressingMode::absolute);    break;
      case 0xEE: INC(addressingMode::absolute);    break;
      case 0xEF: ISC(addressingMode::absolute);    break;
      case 0xF0: BEQ(addressingMode::immediate);   break;
      case 0xF1: SBC(addressingMode::indirectY);   break;
      case 0xF2: STP(addressingMode::implied);     break;
      case 0xF3: ISC(addressingMode::indirectY);   break;
      case 0xF4: NOP(addressingMode::zeroPageX);   break;
      case 0xF5: SBC(addressingMode::zeroPageX);   break;
      case 0xF6: INC(addressingMode::zeroPageX);   break;
      case 0xF7: ISC(addressingMode::zeroPageX);   break;
      case 0xF8: SED(addressingMode::implied);     break;
      case 0xF9: SBC(addressingMode::absoluteY);   break;
      case 0xFA: NOP(addressingMode::implied);     break;
      case 0xFB: ISC(addressingMode::absoluteY);   break;
      case 0xFC: NOP(addressingMode::absoluteX);   break;
      case 0xFD: SBC(addressingMode::absoluteX);   break;
      case 0xFE: INC(addressingMode::absoluteX);   break;
      case 0xFF: ISC(addressingMode::absoluteX);   break;
    };
  }


  //-----------------------------------------
  // Addressing modes
  //-----------------------------------------
  WORD Cpu::indexedInderectAddressing() {
    BYTE arg = readBus(regPC++);
    return readBus((arg + regX) % 256) + readBus((arg + regX + 1) % 256) * 256;
  }

  WORD Cpu::indirectIndexedAddressing() {
    BYTE arg = readBus(regPC++);
    WORD address = readBus(arg) + readBus((arg + 1) % 256) * 256;
    if (((address + regY) & 0xFF00) != (address & 0xFF00))
      pageCrossed = true;
    return address + regY;
  }

  WORD Cpu::zeroPageAddressing() {
    return readBus(regPC++);
  }

  WORD Cpu::zeroPageIndexedAddressing_X() {
    BYTE arg = readBus(regPC++);
    return (arg + regX) % 256;
  }

  WORD Cpu::zeroPageIndexedAddressing_Y() {
    BYTE arg = readBus(regPC++);
    return (arg + regY) % 256;
  }

  WORD Cpu::absoluteAddressing() {
    regPC += 2;
    return readBus16(regPC - 2);
  }

  WORD Cpu::absoluteIndexedAddressing_X() {
    WORD address = absoluteAddressing();
    if (((address + regX) & 0xFF00) != (address & 0xFF00))
      pageCrossed = true;
    return address + regX;
  }

  WORD Cpu::absoluteIndexedAddressing_Y() {
    WORD address = absoluteAddressing();
    if (((address + regY) & 0xFF00) != (address & 0xFF00))
      pageCrossed = true;
    return address + regY;
  }

  WORD Cpu::immediateAddressing() {
    regPC++;
    return regPC - 1;
  }

  WORD Cpu::indirectAddressing() {
    regPC += 2;
    if (readBus(regPC - 2) == 0xFF) {
      return readBus16(regPC - 2) + 1;
    } else {
      WORD address = readBus16(regPC - 2);
      return readBus16(address);
    }
  }

  WORD Cpu::getAddress(addressingMode mode) {
    if (mode == addressingMode::immediate)
      return immediateAddressing();
    if (mode == addressingMode::zeroPage)
      return zeroPageAddressing();
    if (mode == addressingMode::zeroPageX)
      return zeroPageIndexedAddressing_X();
    if (mode == addressingMode::zeroPageY)
      return zeroPageIndexedAddressing_Y();
    if (mode == addressingMode::absolute)
      return absoluteAddressing();
    if (mode == addressingMode::absoluteX)
      return absoluteIndexedAddressing_X();
    if (mode == addressingMode::absoluteY)
      return absoluteIndexedAddressing_Y();
    if (mode == addressingMode::indirectX)
      return indexedInderectAddressing();
    if (mode == addressingMode::indirectY)
      return indirectIndexedAddressing();
    if (mode == addressingMode::indirect)
      return indirectAddressing();
    return -1;
  }

  //-----------------------------------------
  // processor status operations
  //-----------------------------------------
  void Cpu::setCarryFlag(bool value) {
    if (value)
      regP |= CARRY_FLAG;
    else
      regP &= ~CARRY_FLAG;
  }

  void Cpu::setZeroFlag(const BYTE& arg) {
    if (!arg)
      regP |= ZERO_FLAG;
    else
      regP &= ~ZERO_FLAG;
  }

  void Cpu::setInterruptDisableFlag(bool value) {
    if (value)
      regP |= INTERRUPT_DISABLE_FLAG;
    else
      regP &= ~INTERRUPT_DISABLE_FLAG;
  }

  void Cpu::setDecModeFlag(bool value) {
    if (value)
      regP |= DECIMAL_MODE_FLAG;
    else
      regP &= ~DECIMAL_MODE_FLAG;
  }

  void Cpu::setBreakFlag(bool value) {
    if (value)
      regP |= BREAK_FLAG;
    else
      regP &= ~BREAK_FLAG;
  }
  void Cpu::setOverflowFlag(bool value) {
    if (value)
      regP |= OVERFLOW_FLAG;
    else
      regP &= ~OVERFLOW_FLAG;
  }

  void Cpu::setNegativeFlag(const BYTE& arg) {
    regP = (regP & ~NEGATIVE_FLAG) | (arg & NEGATIVE_FLAG) | 0x20;
  }

  //-----------------------------------------
  // stack operations
  //-----------------------------------------
  void Cpu::push(BYTE arg) {
    writeBus(regSP--, arg);
  }

  BYTE Cpu::pull() {
    return readBus(++regSP);
  }

  //-----------------------------------------
  // instructions
  //-----------------------------------------
  void Cpu::BRK(addressingMode mode) {
    ++regPC;
    push((regPC >> 8) & 0xFF);
    push(regPC & 0xFF);
    setBreakFlag(true);
    push(regP | BREAK_FLAG);
    setInterruptDisableFlag(true);
    regPC = readBus16(0xFFFE);
    cycles = 7;
  }

  void Cpu::NOP(addressingMode mode) {
    getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::immediate)
      cycles = 2;
    else if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::zeroPageX)
      cycles = 4;
    else if (mode == addressingMode::implied)
      cycles = 2;
    else if (mode == addressingMode::absolute)
      cycles = 4;
    else if (mode == addressingMode::absoluteX)
      cycles = pageCrossed ? 5 : 4;

  }

  void Cpu::PHP(addressingMode mode) {
    logStatus(mode);
    push(regP | BREAK_FLAG);
    cycles = 3;
  }

  void Cpu::BPL(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    cycles = 2;
    if (!(regP & NEGATIVE_FLAG)) {
      WORD oldRegPC = regPC;
      regPC += (char)readBus(address);
      if ((regPC & 0xFF00) == (oldRegPC & 0xFF00))
        cycles += 1;
      else
        cycles += 2;
    }
  }

  void Cpu::CLC(addressingMode mode) {
    logStatus(mode);
    setCarryFlag(false);
    cycles = 2;
  }

  void Cpu::ORA(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::immediate)
      cycles = 2;
    else if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::zeroPageX)
      cycles = 4;
    else if (mode == addressingMode::zeroPageY)
      cycles = 4;
    else if (mode == addressingMode::absolute)
      cycles = 4;
    else if (mode == addressingMode::absoluteX)
      cycles = pageCrossed ? 5 : 4;
    else if (mode == addressingMode::absoluteY)
      cycles = pageCrossed ? 5 : 4;
    else if (mode == addressingMode::indirectX)
      cycles = 6;
    else if (mode == addressingMode::indirectY)
      cycles = pageCrossed ? 6 : 5;


    regA |= readBus(address);
    setZeroFlag(regA);
    setNegativeFlag(regA);
  }

  void Cpu::STP(addressingMode mode) {
    logStatus(mode);
    stopped = true;
  }

  void Cpu::ASL(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::implied) {
      setCarryFlag(regA & 0X80);
      regA <<= 1;
      setZeroFlag(regA);
      setNegativeFlag(regA);
      cycles = 2;
    } else {
      if (mode == addressingMode::zeroPage)
        cycles = 5;
      else if (mode == addressingMode::zeroPageX)
        cycles = 6;
      else if (mode == addressingMode::absolute)
        cycles = 6;
      else if (mode == addressingMode::absoluteX)
        cycles = 7;
      setCarryFlag(readBus(address) & 0X80);
      writeBus(address, readBus(address) << 1);
      setZeroFlag(readBus(address));
      setNegativeFlag(readBus(address));
    }
  }

  void Cpu::SLO(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::zeroPage)
      cycles = 5;
    else if (mode == addressingMode::zeroPageX)
      cycles = 6;
    else if (mode == addressingMode::zeroPageY)
      cycles = 6;
    else if (mode == addressingMode::absolute)
      cycles = 6;
    else if (mode == addressingMode::absoluteX)
      cycles = 7;
    else if (mode == addressingMode::absoluteY)
      cycles = 7;
    else if (mode == addressingMode::indirectX)
      cycles = 8;
    else if (mode == addressingMode::indirectY)
      cycles = 8;

    setCarryFlag(readBus(address) & 0X80);

    writeBus(address, readBus(address) << 1);

    regA |= readBus(address);

    setZeroFlag(readBus(address));
  }

  void Cpu::ANC(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    cycles = 2;

    regA &= readBus(address);

    regP = (regP & ~CARRY_FLAG) | (regA >> 7);
  }

  void Cpu::JSR(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    cycles = 6;
    --regPC;
    push(regPC >> 8);
    push(regPC & 0xFF);
    regPC = address;
  }

  void Cpu::BIT(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::absolute)
      cycles = 4;

    regP = (regP & ~NEGATIVE_FLAG) | (readBus(address) & NEGATIVE_FLAG);
    regP = (regP & ~OVERFLOW_FLAG) | (readBus(address) & OVERFLOW_FLAG);
    if (!(regA & readBus(address)))
      regP |= ZERO_FLAG;
    else
      regP &= ~ZERO_FLAG;
  }

  void Cpu::PLP(addressingMode mode) {
    logStatus(mode);
    cycles = 4;
    regP = (pull() & ~BREAK_FLAG) | 0x20;
  }

  void Cpu::BMI(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    cycles = 2;
    if (regP & NEGATIVE_FLAG) {
      WORD oldRegPC = regPC;
      regPC += (char)readBus(address);
      if ((regPC & 0xFF00) == (oldRegPC & 0xFF00))
        cycles += 1;
      else
        cycles += 2;
    }
  }

  void Cpu::SEC(addressingMode mode) {
    logStatus(mode);
    cycles = 2;
    regP |= CARRY_FLAG;
  }

  void Cpu::AND(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::immediate)
      cycles = 2;
    else if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::zeroPageX)
      cycles = 4;
    else if (mode == addressingMode::absolute)
      cycles = 4;
    else if (mode == addressingMode::absoluteX)
      cycles = pageCrossed ? 5 : 4;
    else if (mode == addressingMode::absoluteY)
      cycles = pageCrossed ? 5 : 4;
    else if (mode == addressingMode::indirectX)
      cycles = 6;
    else if (mode == addressingMode::indirectY)
      cycles = pageCrossed ? 6 : 5;

    regA &= readBus(address);
    setNegativeFlag(regA);
    setZeroFlag(regA);
  }

  void Cpu::ROL(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::implied) {
      cycles = 2;
      regA = (regA << 1) | (regA >> 7);
      setCarryFlag(regA & 0x01);
      setNegativeFlag(regA);
      setZeroFlag(regA);
    } else {
      if (mode == addressingMode::zeroPage)
        cycles = 5;
      else if (mode == addressingMode::zeroPageX)
        cycles = 6;
      else if (mode == addressingMode::absolute)
        cycles = 6;
      else if (mode == addressingMode::absoluteX)
        cycles = 7;

      bool carryFlagSet = readBus(address) & 0x80;
      writeBus(address, readBus(address) << 1 | (regP & CARRY_FLAG));
      setCarryFlag(carryFlagSet);
      setNegativeFlag(readBus(address));
      setZeroFlag(readBus(address));
    }

  }

  void Cpu::RLA(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::zeroPage)
      cycles = 5;
    else if (mode == addressingMode::zeroPageX)
      cycles = 6;
    else if (mode == addressingMode::absolute)
      cycles = 6;
    else if (mode == addressingMode::absoluteX)
      cycles = 7;
    else if (mode == addressingMode::absoluteY)
      cycles = 7;
    else if (mode == addressingMode::indirectX)
      cycles = 8;
    else if (mode == addressingMode::indirectY)
      cycles = 8;

    bool carryFlagSet = readBus(address) & 0x80;
    writeBus(address, (readBus(address) << 1) | (regP & CARRY_FLAG));
    //write(address, read(address) & regA);
    regA &= readBus(address);
    setCarryFlag(carryFlagSet);
    setNegativeFlag(readBus(address));
    setZeroFlag(readBus(address));
  }

  void Cpu::RTI(addressingMode mode) {
    logStatus(mode);
    cycles = 6;
    regP = pull() | 0x20;
    regPC = pull();
    regPC |= ((WORD)pull()) << 8;
  }

  void Cpu::PHA(addressingMode mode) {
    logStatus(mode);
    cycles = 3;
    push(regA);
  }

  void Cpu::JMP(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::absolute)
      cycles = 3;
    else if (mode == addressingMode::indirect)
      cycles = 5;
    regPC = address;
  }

  void Cpu::BVC(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    cycles = 2;
    if (!(regP & OVERFLOW_FLAG)) {
      WORD oldRegPC = regPC;
      regPC += (char)readBus(address);
      if ((regPC & 0xFF00) == (oldRegPC & 0xFF00))
        cycles += 1;
      else
        cycles += 2;
    }
  }

  void Cpu::CLI(addressingMode mode) {
    logStatus(mode);
    cycles = 2;
    regP &= ~INTERRUPT_DISABLE_FLAG;
  }

  void Cpu::EOR(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::immediate)
      cycles = 2;
    else if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::zeroPageX)
      cycles = 4;
    else if (mode == addressingMode::absolute)
      cycles = 4;
    else if (mode == addressingMode::absoluteX)
      cycles = pageCrossed ? 5 : 4;
    else if (mode == addressingMode::absoluteY)
      cycles = pageCrossed ? 5 : 4;
    else if (mode == addressingMode::indirectX)
      cycles = 6;
    else if (mode = addressingMode::indirectY)
      cycles = pageCrossed ? 6 : 5;

    regA ^= readBus(address);
    setNegativeFlag(regA);
    setZeroFlag(regA);
  }

  void Cpu::LSR(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::implied) {
      cycles = 2;

      setCarryFlag(regA & CARRY_FLAG);
      regA >>= 1;
      setZeroFlag(regA);
      setNegativeFlag(regA);
    } else {
      if (mode == addressingMode::zeroPage)
        cycles = 5;
      else if (mode == addressingMode::zeroPageX)
        cycles = 6;
      else if (mode == addressingMode::absolute)
        cycles = 6;
      else if (mode == addressingMode::absoluteX)
        cycles = 7;

      setCarryFlag(readBus(address) & CARRY_FLAG);
      writeBus(address, readBus(address) >> 1);
      setZeroFlag(readBus(address));
      setNegativeFlag(readBus(address));
    }
  }

  void Cpu::SRE(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::zeroPage)
      cycles = 5;
    else if (mode == addressingMode::zeroPageX)
      cycles = 6;
    else if (mode == addressingMode::absolute)
      cycles = 6;
    else if (mode == addressingMode::absoluteX)
      cycles = 7;
    else if (mode == addressingMode::absoluteY)
      cycles = 7;
    else if (mode == addressingMode::indirectX)
      cycles = 8;
    else if (mode == addressingMode::indirectY)
      cycles = 8;

    setCarryFlag(readBus(address) & 0x01);
    writeBus(address, readBus(address) >> 1);
    regA ^= readBus(address);
    setZeroFlag(regA);
    setNegativeFlag(regA);
  }

  void Cpu::ALR(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    cycles = 2;

    regA &= readBus(address);

    setCarryFlag(regA & 0x01);
    regA >>= 1;
    setZeroFlag(regA);
    setNegativeFlag(regA);
  }

  void Cpu::RTS(addressingMode mode) {
    logStatus(mode);
    cycles = 6;
    regPC = pull();
    regPC += ((WORD)pull() << 8) + 1;
  }

  void Cpu::PLA(addressingMode mode) {
    logStatus(mode);
    cycles = 4;
    regA = pull();
    setZeroFlag(regA);
    setNegativeFlag(regA);
  }

  void Cpu::BVS(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    cycles = 2;
    if (regP & OVERFLOW_FLAG) {
      WORD oldRegPC = regPC;
      regPC += (char)readBus(address);
      if ((regPC & 0xFF00) == (oldRegPC & 0xFF00))
        cycles += 1;
      else
        cycles += 2;
    }
  }

  void Cpu::SEI(addressingMode mode) {
    logStatus(mode);
    cycles = 2;
    setInterruptDisableFlag(true);
  }

  void Cpu::ADC(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::immediate)
      cycles = 2;
    else if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::zeroPageX)
      cycles = 4;
    else if (mode == addressingMode::absolute)
      cycles = 4;
    else if (mode == addressingMode::absoluteX)
      cycles = pageCrossed ? 5 : 4;
    else if (mode == addressingMode::absoluteY)
      cycles = pageCrossed ? 5 : 4;
    else if (mode == addressingMode::indirectX)
      cycles = 6;
    else if (mode == addressingMode::indirectY)
      cycles = pageCrossed ? 6 : 5;


    if (regP & DECIMAL_MODE_FLAG && false) {
      BYTE result = regA + ((readBus(address) % 16) + ((readBus(address) / 16) % 16) * 10) + (regP & CARRY_FLAG ? 1 : 0);
      setOverflowFlag((regA ^ result) & (readBus(address) ^ result) & 0x80);
      regA += result;
      setCarryFlag(regA > 0x99);
    } else {
      BYTE result = regA + readBus(address) + (regP & CARRY_FLAG ? 1 : 0);
      setOverflowFlag((regA ^ result) & (readBus(address) ^ result) & 0x80);
      regA = result;
      setCarryFlag(regA < readBus(address));
    }
    setZeroFlag(regA);
    setNegativeFlag(regA);
  }

  void Cpu::ROR(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::implied) {
      cycles = 2;

      bool shouldSetCarry = regA & 0x01;
      regA = (regA >> 1) | ((regP & CARRY_FLAG) << 7);
      setCarryFlag(shouldSetCarry);
      setNegativeFlag(regA);
      setZeroFlag(regA);
    } else {
      if (mode == addressingMode::zeroPage)
        cycles = 5;
      else if (mode == addressingMode::zeroPageX)
        cycles = 6;
      else if (mode == addressingMode::absolute)
        cycles = 6;
      else if (mode == addressingMode::absoluteX)
        cycles = 7;

      bool shouldSetCarry = readBus(address) & 0x01;
      writeBus(address, (readBus(address) >> 1) | ((regP & CARRY_FLAG) << 7));
      setCarryFlag(shouldSetCarry);
      setNegativeFlag(readBus(address));
      setZeroFlag(readBus(address));
    }
  }

  void Cpu::RRA(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::zeroPage)
      cycles = 5;
    else if (mode == addressingMode::zeroPageX)
      cycles = 6;
    else if (mode == addressingMode::absolute)
      cycles = 6;
    else if (mode == addressingMode::absoluteX)
      cycles = 7;
    else if (mode == addressingMode::absoluteY)
      cycles = 7;
    else if (mode == addressingMode::indirectX)
      cycles = 8;
    else if (mode == addressingMode::indirectY)
      cycles = 8;

    writeBus(address, (readBus(address) >> 1) | ((regP & CARRY_FLAG) << 7));

    if (regP & DECIMAL_MODE_FLAG & false) {
      regA += ((readBus(address) % 16) + ((readBus(address) / 16) % 16) * 10) + (regP & CARRY_FLAG ? 1 : 0);
      setCarryFlag(regA > 0x99);
    } else {
      regA += readBus(address) + 1; // (regP & CARRY_FLAG ? 1 : 0);
      setCarryFlag(regA < readBus(address));
    }
    setZeroFlag(regA);
    setNegativeFlag(regA);
    setOverflowFlag(regA & 0x80);
  }

  void Cpu::ARR(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    cycles = 2;

    regA &= readBus(address);
    regA >>= 1;
    if (regA & 0x10 && regA & 0x20) {
      setCarryFlag(true);
      setOverflowFlag(false);
    } else if (regA & 0x10) {
      setCarryFlag(false);
      setOverflowFlag(true);
    } else if (regA & 0x20) {
      setCarryFlag(true);
      setOverflowFlag(true);
    } else {
      setCarryFlag(false);
      setOverflowFlag(false);
    }
    setNegativeFlag(regA);
    setZeroFlag(regA);
  }

  void Cpu::STY(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::zeroPageX)
      cycles = 4;
    else if (mode == addressingMode::absolute)
      cycles = 4;

    writeBus(address, regY);
  }

  void Cpu::DEY(addressingMode mode) {
    cycles = 2;
    logStatus(mode);
    --regY;
    setNegativeFlag(regY);
    setZeroFlag(regY);
  }

  void Cpu::BCC(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    cycles = 2;
    if (!(regP & CARRY_FLAG)) {
      WORD oldRegPC = regPC;
      regPC += (char)readBus(address);
      if ((regPC & 0xFF00) == (oldRegPC & 0xFF00))
        cycles += 1;
      else
        cycles += 2;
    }
  }

  void Cpu::TYA(addressingMode mode) {
    logStatus(mode);
    cycles = 2;
    regA = regY;
    setNegativeFlag(regA);
    setZeroFlag(regA);
  }

  void Cpu::SHY(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    cycles = 5;
    writeBus(address, regY & (readBus(regPC - 1) + 1));
  }

  void Cpu::STA(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::zeroPageX)
      cycles = 4;
    else if (mode == addressingMode::absolute)
      cycles = 4;
    else if (mode == addressingMode::absoluteX)
      cycles = 5;
    else if (mode == addressingMode::absoluteY)
      cycles = 5;
    else if (mode == addressingMode::indirectX)
      cycles = 6;
    else if (mode == addressingMode::indirectY)
      cycles = 6;

    writeBus(address, regA);
  }

  void Cpu::STX(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::zeroPageY)
      cycles = 4;
    else if (mode == addressingMode::absolute)
      cycles = 4;

    writeBus(address, regX);
  }

  void Cpu::TXA(addressingMode mode) {
    logStatus(mode);
    cycles = 2;

    regA = regX;
    setNegativeFlag(regA);
    setZeroFlag(regA);
  }

  void Cpu::TXS(addressingMode mode) {
    logStatus(mode);
    cycles = 2;

    regSP = regX;
    regSP |= 0x100;
  }

  void Cpu::SHX(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    cycles = 5;
    writeBus(address, regX & (readBus(regPC - 1) + 1));
  }

  void Cpu::SAX(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::zeroPageY)
      cycles = 4;
    else if (mode == addressingMode::indirectX)
      cycles = 6;
    else if (mode == addressingMode::absolute)
      cycles = 4;

    writeBus(address, regA & regX);
  }

  void Cpu::XAA(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    cycles = 2;

    regA = regX;
    regA &= readBus(address);
  }

  void Cpu::AHX(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::absoluteY)
      cycles = 5;
    else if (mode == addressingMode::indirectY)
      cycles = 6;

    writeBus(address, regA & regX & (readBus(regPC - 1) + 1));
  }

  void Cpu::TAS(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    cycles = 5;

    writeBus(address, regA & regX & (readBus(regPC - 1) + 1));
  }

  void Cpu::LDY(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::immediate)
      cycles = 2;
    else if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::zeroPageX)
      cycles = 4;
    else if (mode == addressingMode::absolute)
      cycles = 4;
    else if (mode == addressingMode::absoluteX)
      cycles = pageCrossed ? 5 : 4;

    regY = readBus(address);
    setNegativeFlag(regY);
    setZeroFlag(regY);
  }

  void Cpu::TAY(addressingMode mode) {
    logStatus(mode);
    cycles = 2;

    regY = regA;
    setZeroFlag(regY);
    setNegativeFlag(regY);
  }

  void Cpu::BCS(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    cycles = 2;
    if (regP & CARRY_FLAG) {
      WORD oldRegPC = regPC;
      regPC += (char)readBus(address);
      if ((regPC & 0xFF00) == (oldRegPC & 0xFF00))
        cycles += 1;
      else
        cycles += 2;
    }
  }

  void Cpu::CLV(addressingMode mode) {
    logStatus(mode);
    cycles = 2;
    setOverflowFlag(false);
  }

  void Cpu::LDA(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::immediate)
      cycles = 2;
    else if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::zeroPageX)
      cycles = 4;
    else if (mode == addressingMode::absolute)
      cycles = 4;
    else if (mode == addressingMode::absoluteX)
      cycles = pageCrossed ? 5 : 4;
    else if (mode == addressingMode::absoluteY)
      cycles = pageCrossed ? 5 : 4;
    else if (mode == addressingMode::indirectX)
      cycles = 6;
    else if (mode == addressingMode::indirectY)
      cycles = pageCrossed ? 6 : 5;

    regA = readBus(address);
    setNegativeFlag(regA & 0x80);
    setZeroFlag(regA);
  }

  void Cpu::LDX(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::immediate)
      cycles = 2;
    else if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::zeroPageY)
      cycles = 4;
    else if (mode == addressingMode::absolute)
      cycles = 4;
    else if (mode == addressingMode::absoluteY)
      cycles = pageCrossed ? 5 : 4;

    regX = readBus(address);
    setNegativeFlag(regX);
    setZeroFlag(regX);
  }

  void Cpu::TAX(addressingMode mode) {
    logStatus(mode);
    cycles = 2;
    regX = regA;
    setZeroFlag(regA);
    setNegativeFlag(regA);
  }

  void Cpu::TSX(addressingMode mode) {
    logStatus(mode);
    cycles = 2;
    regX = (BYTE)regSP;
    setZeroFlag(regX);
    setNegativeFlag(regX & 0x80);
  }

  void Cpu::LAX(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::zeroPageY)
      cycles = 4;
    else if (mode == addressingMode::absolute)
      cycles = 4;
    else if (mode == addressingMode::absoluteY)
      cycles = pageCrossed ? 5 : 4;
    else if (mode == addressingMode::indirectX)
      cycles = 6;
    else if (mode == addressingMode::indirectY)
      cycles = pageCrossed ? 6 : 5;;

    regA = readBus(address);
    regX = regA;
    setZeroFlag(regA);
    setNegativeFlag(regA);
  }

  void Cpu::LAS(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    cycles = pageCrossed ? 5 : 4;

    regA = regX = regSP &= readBus(address);
    setNegativeFlag(regA);
    setZeroFlag(regA);
  }

  void Cpu::CPY(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::immediate)
      cycles = 2;
    else if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::absolute)
      cycles = 4;

    setNegativeFlag(regY - readBus(address));
    setZeroFlag(regY - readBus(address));
    setCarryFlag(readBus(address) <= regY);
  }

  void Cpu::INY(addressingMode mode) {
    logStatus(mode);
    cycles = 2;
    ++regY;
    setZeroFlag(regY);
    setNegativeFlag(regY & 0x80);
  }

  void Cpu::BNE(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    cycles = 2;
    if (!(regP & ZERO_FLAG)) {
      WORD oldRegPC = regPC;
      regPC += (char)readBus(address);
      if ((regPC & 0xFF00) == (oldRegPC & 0xFF00))
        cycles += 1;
      else
        cycles += 2;
    }
  }

  void Cpu::CLD(addressingMode mode) {
    cycles = 2;
    logStatus(mode);
    regP &= ~DECIMAL_MODE_FLAG;
  }

  void Cpu::CMP(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::immediate)
      cycles = 2;
    else if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::zeroPageX)
      cycles = 4;
    else if (mode == addressingMode::absolute)
      cycles = 4;
    else if (mode == addressingMode::absoluteX)
      cycles = pageCrossed ? 5 : 4;
    else if (mode == addressingMode::absoluteY)
      cycles = pageCrossed ? 5 : 4;
    else if (mode == addressingMode::indirectX)
      cycles = 6;
    else if (mode == addressingMode::indirectY)
      cycles = pageCrossed ? 6 : 5;

    setNegativeFlag(regA - readBus(address));
    setZeroFlag(regA - readBus(address));
    setCarryFlag(readBus(address) <= regA);
  }

  void Cpu::DEC(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::zeroPage)
      cycles = 5;
    else if (mode == addressingMode::zeroPageX)
      cycles = 6;
    else if (mode == addressingMode::absolute)
      cycles = 6;
    else if (mode == addressingMode::absoluteX)
      cycles = 7;

    writeBus(address, readBus(address) - 1);
    setZeroFlag(readBus(address));
    setNegativeFlag(readBus(address));
  }

  void Cpu::DEX(addressingMode mode) {
    logStatus(mode);
    cycles = 2;
    --regX;
    setZeroFlag(regX);
    setNegativeFlag(regX & 0x80);
  }

  void Cpu::DCP(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::zeroPage)
      cycles = 5;
    else if (mode == addressingMode::zeroPageX)
      cycles = 6;
    else if (mode == addressingMode::absolute)
      cycles = 6;
    else if (mode == addressingMode::absoluteX)
      cycles = 7;
    else if (mode == addressingMode::absoluteY)
      cycles = 7;
    else if (mode == addressingMode::indirectX)
      cycles = 8;
    else if (mode == addressingMode::indirectY)
      cycles = 8;

    writeBus(address, readBus(address) - 1);
    setNegativeFlag(regA - readBus(address));
    setZeroFlag(regA - readBus(address));
    setCarryFlag(readBus(address) <= regA);
  }

  void Cpu::AXS(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::zeroPageY)
      cycles = 4;
    else if (mode == addressingMode::indirectX)
      cycles = 6;
    else if (mode == addressingMode::absolute)
      cycles = 4;

    writeBus(address, regA & regX);
  }

  void Cpu::CPX(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::immediate)
      cycles = 2;
    else if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::absolute)
      cycles = 4;

    setNegativeFlag(regX - readBus(address));
    setZeroFlag(regX - readBus(address));
    setCarryFlag(readBus(address) <= regX);
  }

  void Cpu::INX(addressingMode mode) {
    logStatus(mode);
    cycles = 2;
    ++regX;
    setZeroFlag(regX);
    setNegativeFlag(regX & 0x80);
  }

  void Cpu::BEQ(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    cycles = 2;
    if (regP & ZERO_FLAG) {
      WORD oldRegPC = regPC;
      regPC += (char)readBus(address);
      if ((regPC & 0xFF00) == (oldRegPC & 0xFF00))
        cycles += 1;
      else
        cycles += 2;
    }
  }

  void Cpu::SED(addressingMode mode) {
    logStatus(mode);
    cycles = 2;
    regP |= DECIMAL_MODE_FLAG;
  }

  void Cpu::SBC(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::immediate)
      cycles = 2;
    else if (mode == addressingMode::zeroPage)
      cycles = 3;
    else if (mode == addressingMode::zeroPageX)
      cycles = 4;
    else if (mode == addressingMode::absolute)
      cycles = 4;
    else if (mode == addressingMode::absoluteX)
      cycles = pageCrossed ? 5 : 4;
    else if (mode == addressingMode::absoluteY)
      cycles = pageCrossed ? 5 : 4;
    else if (mode == addressingMode::indirectX)
      cycles = 6;
    else if (mode == addressingMode::indirectY)
      cycles = pageCrossed ? 6 : 5;

    unsigned int temp = regA - readBus(address) - (regP & CARRY_FLAG ? 0 : 1);
    setNegativeFlag(temp);
    setZeroFlag(temp & 0xff);	/* Sign and Zero are invalid in decimal mode */
    setOverflowFlag(((regA ^ temp) & 0x80) && ((regA ^ readBus(address)) & 0x80));
    if (regP & DECIMAL_MODE_FLAG && false) {
      if (((regA & 0xf) - (regP & CARRY_FLAG ? 0 : 1)) < (readBus(address) & 0xf)) /* EP */ temp -= 6;
      if (temp > 0x99) temp -= 0x60;
    }
    setCarryFlag(temp < 0x100);
    regA = (temp & 0xff);
  }

  void Cpu::INC(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::zeroPage)
      cycles = 5;
    else if (mode == addressingMode::zeroPageX)
      cycles = 6;
    else if (mode == addressingMode::absolute)
      cycles = 6;
    else if (mode == addressingMode::absoluteX)
      cycles = 7;

    writeBus(address, readBus(address) + 1);
    setZeroFlag(readBus(address));
    setNegativeFlag(readBus(address) & 0x80);
  }

  void Cpu::ISC(addressingMode mode) {
    WORD address = getAddress(mode);
    logStatus(mode);
    if (mode == addressingMode::zeroPage)
      cycles = 5;
    else if (mode == addressingMode::zeroPageX)
      cycles = 6;
    else if (mode == addressingMode::absolute)
      cycles = 6;
    else if (mode == addressingMode::absoluteX)
      cycles = 7;
    else if (mode == addressingMode::absoluteY)
      cycles = 7;
    else if (mode == addressingMode::indirectX)
      cycles = 8;
    else if (mode == addressingMode::indirectY)
      cycles = 8;

    writeBus(address, readBus(address) + 1);
    unsigned int temp = regA - readBus(address) - (regP & CARRY_FLAG ? 0 : 1);
    setNegativeFlag(temp);
    setZeroFlag(temp & 0xff);	/* Sign and Zero are invalid in decimal mode */
    setOverflowFlag(((regA ^ temp) & 0x80) && ((regA ^ readBus(address)) & 0x80));
    if (regP & DECIMAL_MODE_FLAG && false) {
      if (((regA & 0xf) - (regP & CARRY_FLAG ? 0 : 1)) < (readBus(address) & 0xf)) /* EP */ temp -= 6;
      if (temp > 0x99) temp -= 0x60;
    }
    setCarryFlag(temp < 0x100);
    regA = (temp & 0xff);
  }

  // This method is for logging the CPU status after every instruction.
  // To enable logging, set the LOGGING flag to 1 at the top of this file.
  // If you wish to compare the output to the nestest log, set the NESTEST_START
  // flag to 1 at the top of this file. 
  // run like NES > out.txt.
  // http://www.qmtpro.com/~nes/misc/nestest.log
  // Remove the disassembly from the log before comparing this output to it.
  void Cpu::logStatus(addressingMode mode) {
#if LOGGING
    if (mode == addressingMode::immediate) {
      printf("%04X  ", regPC - 2);
      printf("%02X %02X     ", readBus(regPC - 2), readBus(regPC - 1));
    }
    if (mode == addressingMode::zeroPage) {
      printf("%04X  ", regPC - 2);
      printf("%02X %02X     ", readBus(regPC - 2), readBus(regPC - 1));
    }
    if (mode == addressingMode::zeroPageX) {
      printf("%04X  ", regPC - 2);
      printf("%02X %02X     ", readBus(regPC - 2), readBus(regPC - 1));
    }
    if (mode == addressingMode::zeroPageY) {
      printf("%04X  ", regPC - 2);
      printf("%02X %02X     ", readBus(regPC - 2), readBus(regPC - 1));
    }
    if (mode == addressingMode::absolute) {
      printf("%04X  ", regPC - 3);
      printf("%02X %02X %02X  ", readBus(regPC - 3), readBus(regPC - 2), readBus(regPC - 1));
    }
    if (mode == addressingMode::absoluteX) {
      printf("%04X  ", regPC - 3);
      printf("%02X %02X %02X  ", readBus(regPC - 3), readBus(regPC - 2), readBus(regPC - 1));
    }
    if (mode == addressingMode::absoluteY) {
      printf("%04X  ", regPC - 3);
      printf("%02X %02X %02X  ", readBus(regPC - 3), readBus(regPC - 2), readBus(regPC - 1));
    }
    if (mode == addressingMode::indirectX) {
      printf("%04X  ", regPC - 2);
      printf("%02X %02X     ", readBus(regPC - 2), readBus(regPC - 1));
    }
    if (mode == addressingMode::indirectY) {
      printf("%04X  ", regPC - 2);
      printf("%02X %02X     ", readBus(regPC - 2), readBus(regPC - 1));
    }
    if (mode == addressingMode::indirect) {
      printf("%04X  ", regPC - 3);
      printf("%02X %02X %02X  ", readBus(regPC - 3), readBus(regPC - 2), readBus(regPC - 1));
    }
    if (mode == addressingMode::implied) {
      printf("%04X  ", regPC - 1);
      printf("%02X        ", readBus(regPC - 1));
    }
    printf("A:%02X ", regA);
    printf("X:%02X ", regX);
    printf("Y:%02X ", regY);
    printf("P:%02X ", regP);
    printf("SP:%02X ", (BYTE)regSP);
    printf("CYC:%*d", 3, ppuCycles);
    printf("\n");
#endif
  }

}