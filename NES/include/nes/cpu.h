#pragma once

#include <nes\common\globals.h>
#include <nes\cpu_bus.h>
#include <nes\interrupt_bus.h>

namespace nes {

  class Cpu {
  public:
    Cpu(CpuBus & bus, InterruptBus & interruptBus);

    void reset();
    bool isStopped();
    void tick();

  private:
    void executeNextInstruction();

    // memory 
    BYTE ram[2048];

    CpuBus * bus;
    InterruptBus * interruptBus;

    // These map addresses accordingly
    BYTE read(WORD address);
    WORD read16(WORD address);
    void write(WORD address, BYTE value);

    // registers
    WORD regPC; // program counter
    WORD regSP; // stack pointer
    BYTE regA;  // Accumulator
    BYTE regX;  // index register X
    BYTE regY;  // index register Y
    BYTE regP;  // status register

    int cycles;
    bool stopped;
    bool pageCrossed;

    // addressing modes
    enum addressingMode {
      implied,
      indirectX,
      indirectY,
      zeroPage,
      zeroPageX,
      zeroPageY,
      immediate,
      absolute,
      absoluteX,
      absoluteY,
      indirect
    };

    // gets address
    WORD getAddress(addressingMode mode);
    WORD indexedInderectAddressing();
    WORD indirectIndexedAddressing();
    WORD zeroPageAddressing();
    WORD zeroPageIndexedAddressing_X();
    WORD zeroPageIndexedAddressing_Y();
    WORD immediateAddressing();
    WORD absoluteAddressing();
    WORD absoluteIndexedAddressing_X();
    WORD absoluteIndexedAddressing_Y();
    WORD indirectAddressing();

    // processor status operations
    void setCarryFlag(bool value);
    void setZeroFlag(const BYTE& arg);
    void setInterruptDisableFlag(bool value);
    void setDecModeFlag(bool value);
    void setBreakFlag(bool value);
    void setOverflowFlag(bool value);
    void setNegativeFlag(const BYTE& arg);

    // stack operations
    void push(BYTE arg);
    BYTE pull();

    // instructions
    void BRK(addressingMode mode);
    void NOP(addressingMode mode);
    void PHP(addressingMode mode);
    void BPL(addressingMode mode);
    void CLC(addressingMode mode);
    void ORA(addressingMode mode);
    void STP(addressingMode mode);
    void ASL(addressingMode mode);
    void SLO(addressingMode mode);
    void ANC(addressingMode mode);
    void JSR(addressingMode mode);
    void BIT(addressingMode mode);
    void PLP(addressingMode mode);
    void BMI(addressingMode mode);
    void SEC(addressingMode mode);
    void AND(addressingMode mode);
    void ROL(addressingMode mode);
    void RLA(addressingMode mode);
    void RTI(addressingMode mode);
    void PHA(addressingMode mode);
    void JMP(addressingMode mode);
    void BVC(addressingMode mode);
    void CLI(addressingMode mode);
    void EOR(addressingMode mode);
    void LSR(addressingMode mode);
    void SRE(addressingMode mode);
    void ALR(addressingMode mode);
    void RTS(addressingMode mode);
    void PLA(addressingMode mode);
    void BVS(addressingMode mode);
    void SEI(addressingMode mode);
    void ADC(addressingMode mode);
    void ROR(addressingMode mode);
    void RRA(addressingMode mode);
    void ARR(addressingMode mode);
    void STY(addressingMode mode);
    void DEY(addressingMode mode);
    void BCC(addressingMode mode);
    void TYA(addressingMode mode);
    void SHY(addressingMode mode);
    void STA(addressingMode mode);
    void STX(addressingMode mode);
    void TXA(addressingMode mode);
    void TXS(addressingMode mode);
    void SHX(addressingMode mode);
    void SAX(addressingMode mode);
    void XAA(addressingMode mode);
    void AHX(addressingMode mode);
    void TAS(addressingMode mode);
    void LDY(addressingMode mode);
    void TAY(addressingMode mode);
    void BCS(addressingMode mode);
    void CLV(addressingMode mode);
    void LDA(addressingMode mode);
    void LDX(addressingMode mode);
    void TAX(addressingMode mode);
    void TSX(addressingMode mode);
    void LAX(addressingMode mode);
    void LAS(addressingMode mode);
    void CPY(addressingMode mode);
    void INY(addressingMode mode);
    void BNE(addressingMode mode);
    void CLD(addressingMode mode);
    void CMP(addressingMode mode);
    void DEC(addressingMode mode);
    void DEX(addressingMode mode);
    void DCP(addressingMode mode);
    void AXS(addressingMode mode);
    void CPX(addressingMode mode);
    void INX(addressingMode mode);
    void BEQ(addressingMode mode);
    void SED(addressingMode mode);
    void SBC(addressingMode mode);
    void INC(addressingMode mode);
    void ISC(addressingMode mode);

    // for debugging
    void logStatus(addressingMode mode);
  };

}