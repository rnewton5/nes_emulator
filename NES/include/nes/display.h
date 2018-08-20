#pragma once

namespace nes {

  class Display {
  public:
    int * getRaster();
    void draw(int position, int red, int green, int blue);

  private:
    static const int HEIGHT = 240;
    static const int WIDTH = 256;

    int raster[WIDTH * HEIGHT * 3];

  };
}
