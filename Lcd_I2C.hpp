#pragma once

#include "asm.hpp"
#include "firmware.h"

#include <Print.h>
#include <Wire.h>

// commands
#define LCD_CLEARDISPLAY   0x01
#define LCD_RETURNHOME     0x02
#define LCD_ENTRYMODESET   0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT    0x10
#define LCD_FUNCTIONSET    0x20
#define LCD_SETCGRAMADDR   0x40
#define LCD_SETDDRAMADDR   0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON  0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON   0x02
#define LCD_CURSOROFF  0x00
#define LCD_BLINKON    0x01
#define LCD_BLINKOFF   0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE  0x00
#define LCD_MOVERIGHT   0x04
#define LCD_MOVELEFT    0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE    0x08
#define LCD_1LINE    0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS  0x00

// flags for backlight control
#define LCD_BACKLIGHT   0x08
#define LCD_NOBACKLIGHT 0x00

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

template <uint8_t address, uint8_t width, uint8_t height>
class LcdI2c : public Print {
  uint8_t backlight_ = 0;
  uint8_t function_ = 0;
  uint8_t control_ = 0;
  uint8_t mode_ = 0;
public:

  LcdI2c() {
    debug(F("LcdI2c()"));
  }

  ~LcdI2c() {
    debug(F("~LcdI2c()"));
  }

  void begin() {
    if(height > 1) {
      function_ |= LCD_2LINE;
    }

    delay(50);

    //Now we pull both RS and R/W low to begin commands
    sendRaw(0);
    delay(1000);

    //We start in 8bit mode, try to set 4 bit mode
    sendClock(0x03 << 4);
    delayMicroseconds(4500); // wait min 4.1ms

    //Second try
    sendClock(0x03 << 4);
    delayMicroseconds(4500); // wait min 4.1ms

    //Third try
    sendClock(0x03 << 4);
    delayMicroseconds(150);

    //Finally, set to 4-bit interface
    sendClock(0x02 << 4);

    //Set # lines, font size, etc.
    command(LCD_FUNCTIONSET | function_);

    //Turn the display on with no cursor or blinking default
    control_ = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    enableDisplay(true);

    // clear it off
    clear();

    // Initialize to default text direction (for roman languages)
    mode_ = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

    // set the entry mode
    command(LCD_ENTRYMODESET | mode_);

    home();
  }

  void end() {
    backlight_ = 0;
    function_ = 0;
    control_ = 0;
    mode_ = 0;
  }

  void clear() {
    command(LCD_CLEARDISPLAY);
    delayMicroseconds(2000);
  }

  void home() {
    command(LCD_RETURNHOME);
    delayMicroseconds(2000);
  }

  void enableBacklight(bool val) {
    backlight_ = val ? LCD_BACKLIGHT : LCD_NOBACKLIGHT;
    sendRaw(0);
  }

  void enableDisplay(bool val) {
    if(val) {
      control_ |= LCD_DISPLAYON;
    }
    else {
      control_ &= ~LCD_DISPLAYON;
    }
    command(LCD_DISPLAYCONTROL | control_);
  }

  void enableCursor(bool val) {
    if(val) {
      control_ |= LCD_CURSORON;
    }
    else {
      control_ &= ~LCD_CURSORON;
    }
    command(LCD_DISPLAYCONTROL | control_);
  }

  void enableBlink(bool val) {
    if(val) {
      control_ |= LCD_BLINKON;
    }
    else {
      control_ &= ~LCD_BLINKON;
    }
    command(LCD_DISPLAYCONTROL | control_);
  }

  void seekCursor(const uint8_t x, const uint8_t y) {
    constexpr int row_offsets[] = {0x00, 0x40, 0x14, 0x54};
    command(LCD_SETDDRAMADDR | (x + row_offsets[y]));
  }

  void loadCustomChar(const uint8_t num, const uint8_t* ch) {
    num &= 0x7; // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (num << 3));
    for(uint8_t i = 0; i < 8; ++i)
      write(ch[i]);
  }

  void command(const uint8_t com) {
    send(com, 0);
  }

  void send(const uint8_t value, const uint8_t mode) {
    const uint8_t high = value & 0xF0;
    const uint8_t low = (value << 4) & 0xF0;
    sendClock(high | mode);
    sendClock(low | mode);
  }

  void sendClock(const uint8_t val) {
    sendRaw(val | En);
    delayMicroseconds(1);
    sendRaw(val & ~En);
    delayMicroseconds(50);
  }

  void sendRaw(const uint8_t val) {
    Wire.beginTransmission(address);
    Wire.write(val | backlight_);
    Wire.endTransmission();
  }

  virtual size_t write(const uint8_t data) override {
    send(data, Rs);
    return 1;
  }
};