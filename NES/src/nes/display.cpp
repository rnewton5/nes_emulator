#include <nes\display.h>

namespace nes {

  int * Display::getRaster() {
    return raster;
  }

  void Display::draw(int position, int red, int green, int blue) {
    position *= 3;
    raster[position + 0] = red;
    raster[position + 1] = green;
    raster[position + 2] = blue;
  }

}