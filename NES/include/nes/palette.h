#pragma once

#include <nes/common/globals.h>

namespace nes {

  struct color {
    BYTE red;
    BYTE green;
    BYTE blue;
  };

  class Palette {
  public:
    Palette();

    color getColor(int index);

  private:
    color colors[64];

  };
}