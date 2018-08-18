#include <nes/palette.h>

namespace nes {

  Palette::Palette() {
    colors[ 0].red = 0x7C; colors[ 0].green = 0x7C; colors[ 0].blue = 0x7C;
    colors[ 1].red = 0x00; colors[ 1].green = 0x00; colors[ 1].blue = 0xFC;
    colors[ 2].red = 0x00; colors[ 2].green = 0x00; colors[ 2].blue = 0xBC;
    colors[ 3].red = 0x44; colors[ 3].green = 0x28; colors[ 3].blue = 0xBC;
    colors[ 4].red = 0x94; colors[ 4].green = 0x00; colors[ 4].blue = 0x84;
    colors[ 5].red = 0xA8; colors[ 5].green = 0x00; colors[ 5].blue = 0x20;
    colors[ 6].red = 0xA8; colors[ 6].green = 0x10; colors[ 6].blue = 0x00;
    colors[ 7].red = 0x88; colors[ 7].green = 0x14; colors[ 7].blue = 0x00;
    colors[ 8].red = 0x50; colors[ 8].green = 0x30; colors[ 8].blue = 0x00;
    colors[ 9].red = 0x00; colors[ 9].green = 0x78; colors[ 9].blue = 0x00;
    colors[10].red = 0x00; colors[10].green = 0x68; colors[10].blue = 0x00;
    colors[11].red = 0x00; colors[11].green = 0x58; colors[11].blue = 0x00;
    colors[12].red = 0x00; colors[12].green = 0x40; colors[12].blue = 0x58;
    colors[13].red = 0x00; colors[13].green = 0x00; colors[13].blue = 0x00;
    colors[14].red = 0x00; colors[14].green = 0x00; colors[14].blue = 0x00;
    colors[15].red = 0x00; colors[15].green = 0x00; colors[15].blue = 0x00;
    colors[16].red = 0xBC; colors[16].green = 0xBC; colors[16].blue = 0xBC;
    colors[17].red = 0x00; colors[17].green = 0x78; colors[17].blue = 0xF8;
    colors[18].red = 0x00; colors[18].green = 0x58; colors[18].blue = 0xF8;
    colors[19].red = 0x68; colors[19].green = 0x44; colors[19].blue = 0xFC;
    colors[20].red = 0xD8; colors[20].green = 0x00; colors[20].blue = 0xCC;
    colors[21].red = 0xE4; colors[21].green = 0x00; colors[21].blue = 0x58;
    colors[22].red = 0xF8; colors[22].green = 0x38; colors[22].blue = 0x00;
    colors[23].red = 0xE4; colors[23].green = 0x5C; colors[23].blue = 0x10;
    colors[24].red = 0xAC; colors[24].green = 0x7C; colors[24].blue = 0x00;
    colors[25].red = 0x00; colors[25].green = 0xB8; colors[25].blue = 0x00;
    colors[26].red = 0x00; colors[26].green = 0xA8; colors[26].blue = 0x00;
    colors[27].red = 0x00; colors[27].green = 0xA8; colors[27].blue = 0x44;
    colors[28].red = 0x00; colors[28].green = 0x88; colors[28].blue = 0x88;
    colors[29].red = 0x00; colors[29].green = 0x00; colors[29].blue = 0x00;
    colors[30].red = 0x00; colors[30].green = 0x00; colors[30].blue = 0x00;
    colors[31].red = 0x00; colors[31].green = 0x00; colors[31].blue = 0x00;
    colors[32].red = 0xF8; colors[32].green = 0xF8; colors[32].blue = 0xF8;
    colors[33].red = 0x3C; colors[33].green = 0xBC; colors[33].blue = 0xFC;
    colors[34].red = 0x68; colors[34].green = 0x88; colors[34].blue = 0xFC;
    colors[35].red = 0x98; colors[35].green = 0x78; colors[35].blue = 0xF8;
    colors[36].red = 0xF8; colors[36].green = 0x78; colors[36].blue = 0xF8;
    colors[37].red = 0xF8; colors[37].green = 0x58; colors[37].blue = 0x98;
    colors[38].red = 0xF8; colors[38].green = 0x78; colors[38].blue = 0x58;
    colors[39].red = 0xFC; colors[39].green = 0xA0; colors[39].blue = 0x44;
    colors[40].red = 0xF8; colors[40].green = 0xB8; colors[40].blue = 0x00;
    colors[41].red = 0xB8; colors[41].green = 0xF8; colors[41].blue = 0x18;
    colors[42].red = 0x58; colors[42].green = 0xD8; colors[42].blue = 0x54;
    colors[43].red = 0x58; colors[43].green = 0xF8; colors[43].blue = 0x98;
    colors[44].red = 0x00; colors[44].green = 0xE8; colors[44].blue = 0xD8;
    colors[45].red = 0x78; colors[45].green = 0x78; colors[45].blue = 0x78;
    colors[46].red = 0x00; colors[46].green = 0x00; colors[46].blue = 0x00;
    colors[47].red = 0x00; colors[47].green = 0x00; colors[47].blue = 0x00;
    colors[48].red = 0xFC; colors[48].green = 0xFC; colors[48].blue = 0xFC;
    colors[49].red = 0xA4; colors[49].green = 0xE4; colors[49].blue = 0xFC;
    colors[50].red = 0xB8; colors[50].green = 0xB8; colors[50].blue = 0xF8;
    colors[51].red = 0xD8; colors[51].green = 0xB8; colors[51].blue = 0xF8;
    colors[52].red = 0xF8; colors[52].green = 0xB8; colors[52].blue = 0xF8;
    colors[53].red = 0xF8; colors[53].green = 0xA4; colors[53].blue = 0xC0;
    colors[54].red = 0xF0; colors[54].green = 0xD0; colors[54].blue = 0xB0;
    colors[55].red = 0xFC; colors[55].green = 0xE0; colors[55].blue = 0xA8;
    colors[56].red = 0xF8; colors[56].green = 0xD8; colors[56].blue = 0x78;
    colors[57].red = 0xD8; colors[57].green = 0xF8; colors[57].blue = 0x78;
    colors[58].red = 0xB8; colors[58].green = 0xF8; colors[58].blue = 0xB8;
    colors[59].red = 0xB8; colors[59].green = 0xF8; colors[59].blue = 0xD8;
    colors[60].red = 0x00; colors[60].green = 0xFC; colors[60].blue = 0xFC;
    colors[61].red = 0xF8; colors[61].green = 0xD8; colors[61].blue = 0xF8;
    colors[62].red = 0x00; colors[62].green = 0x00; colors[62].blue = 0x00;
    colors[63].red = 0x00; colors[63].green = 0x00; colors[63].blue = 0x00;
  }

  color Palette::getColor(int index) {
    return colors[index];
  }

}