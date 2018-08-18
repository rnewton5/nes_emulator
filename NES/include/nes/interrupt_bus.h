#pragma once

namespace nes {

  class InterruptBus {
  public:

    void setNmi(bool value);
    void setIrq(bool value);

    bool getNmi();
    bool getIrq();

  private:
    bool nmiInterrupt;
    bool irqInterrupt;
  };

}
