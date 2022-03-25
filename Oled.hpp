#pragma once

#include "firmware.h"
#include <inttypes.h>

#define SSD1306_SETLOWCOLUMN        0x00
#define SSD1306_SETHIGHCOLUMN       0x10
#define SSD1306_MEMORYMODE          0x20
#define SSD1306_COLUMNADDR          0x21
#define SSD1306_PAGEADDR            0x22
#define SSD1306_SETSTARTLINE        0x40
#define SSD1306_DEFAULT_ADDRESS     0x78
#define SSD1306_SETCONTRAST         0x81
#define SSD1306_CHARGEPUMP          0x8D
#define SSD1306_SEGREMAP            0xA0
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON        0xA5
#define SSD1306_NORMALDISPLAY       0xA6
#define SSD1306_INVERTDISPLAY       0xA7
#define SSD1306_SETMULTIPLEX        0xA8
#define SSD1306_DISPLAYOFF          0xAE
#define SSD1306_DISPLAYON           0xAF
#define SSD1306_SETPAGE             0xB0
#define SSD1306_COMSCANINC          0xC0
#define SSD1306_COMSCANDEC          0xC8
#define SSD1306_SETDISPLAYOFFSET    0xD3
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5
#define SSD1306_SETPRECHARGE        0xD9
#define SSD1306_SETCOMPINS          0xDA
#define SSD1306_SETVCOMDETECT       0xDB

#define SSD1306_SWITCHCAPVCC      0x02
#define SSD1306_NOP               0xE3

#define HORIZONTAL_ADDRESSING_MODE   0x00
#define VERTICAL_ADDRESSING_MODE     0x01
#define PAGE_ADDRESSING_MODE         0x02

constexpr uint8_t SSD1306_InitSequence[] = {
  SSD1306_DISPLAYOFF,                             //display off
  SSD1306_MEMORYMODE, HORIZONTAL_ADDRESSING_MODE, //Page Addressing mode
  SSD1306_COMSCANDEC,                             //Scan from 127 to 0 (Reverse scan)
  SSD1306_SETSTARTLINE | 0x00,                    //First line to start scanning from
  SSD1306_SETCONTRAST, 0x7F,                      //contast value to 0x7F according to datasheet
  SSD1306_SEGREMAP | 0x01,                        //Use reverse mapping. 0x00 - is normal mapping
  SSD1306_NORMALDISPLAY,                          //Normal display
  SSD1306_SETMULTIPLEX, 63,                       //Reset to default MUX. See datasheet
  SSD1306_SETDISPLAYOFFSET, 0x00,                 //no offset
  SSD1306_SETDISPLAYCLOCKDIV, 0x80,               //set to default ratio/osc frequency
  SSD1306_SETPRECHARGE, 0x22,                     //switch precharge to 0x22 // 0xF1
  SSD1306_SETCOMPINS, 0x12,                       //set divide ratio
  SSD1306_SETVCOMDETECT, 0x20,                    //vcom deselect to 0x20 // 0x40
  SSD1306_CHARGEPUMP, 0x14,                       //Enable charge pump
  SSD1306_DISPLAYALLON_RESUME,                    //idk
  SSD1306_DISPLAYON,                              //display on
};

class Oled {
  uint8_t* buffer_ = nullptr;
public:

  Oled();

  virtual ~Oled();

  void begin();

  void end();

  virtual void sendSingle(const uint8_t command);

  virtual void sendSequence(const uint8_t* sequence, const uint8_t lenght);
};