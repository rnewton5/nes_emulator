#include <nes/interrupt_bus.h>

namespace nes {

  void InterruptBus::setNmi(bool value) {
    nmiInterrupt = value;
  }

  void InterruptBus::setIrq(bool value) {
    irqInterrupt = value;
  }

  bool InterruptBus::getNmi() {
    return nmiInterrupt;
  }

  bool InterruptBus::getIrq() {
    return irqInterrupt;
  }

}
