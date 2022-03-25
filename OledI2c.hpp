#pragma once

#ifdef __INTELLISENSE__
#include "asm.hpp"
#endif

#include "Oled.hpp"
#include <Wire.h>

template <uint8_t address>
class OledI2c : public Oled {
public:

  OledI2c() {
    debug(F("OledI2c()"));
  }

  ~OledI2c() {
    debug(F("~OledI2c()"));
    end();
  }

  virtual void sendSingle(const uint8_t command) override {
    Wire.beginTransmission(address);
    Wire.write(0x00);
    Wire.endTransmission();
  }

  virtual void sendSequence(const uint8_t* sequence, const uint8_t lenght) override {
    Wire.beginTransmission(address);
    Wire.write(0x00);
    for(uint8_t i = 0; i < lenght; ++i) {
      Wire.write(sequence[i]);
    }
    Wire.endTransmission();
  }

  void clear(bool color = false) {
    memset(buffer_, color ? 255 : 0, 1024);
  }

  void drawPixel(const uint8_t x, const uint8_t y, const bool color) {
    if(color) {
      buffer_[x + (y / 8) * 128] |= (1 << (y & 7));
    }
    else {
      buffer_[x + (y / 8) * 128] &= ~(1 << (y & 7));
    }
  }

  void drawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const bool color) {
    int16_t XLength;
    int16_t YLength;
    int8_t XInc;
    int8_t YInc;
    int16_t Diff;
    int16_t Tmp;

    if(x0 < x1) {
      XLength = x1 - x0;
      XInc = 1;
    }
    else {
      XLength = x0 - x1;
      XInc = -1;
    }
    if(y0 < y1) {
      YLength = y1 - y0;
      YInc = 1;
    }
    else {
      YLength = y0 - y1;
      YInc = -1;
    }
    Diff = XLength - YLength;

    drawPixel(x0, x1, color);

    while((x0 != x1) || (y0 != y1)) {
      Tmp = Diff << 1;
      if(Tmp > (-YLength)) {
        Diff -= YLength;
        x0 += XInc;
      }
      if(Tmp < XLength) {
        Diff += XLength;
        y0 += YInc;
      }
      drawPixel(x0, y0, color);
    }
  }

  void drawRect(const uint8_t x0, const uint8_t y0, const uint8_t x1, const uint8_t y1, const bool color) {
    drawLine(x0, y0, x1, y0, color);
    drawLine(x0, y1, x1, y1, color);
    drawLine(x0, y0, x0, y1, color);
    drawLine(x1, y0, x1, y1, color);
  }

  void drawFillRect(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const bool color) {
    if(x0 > x1) {
      uint8_t b = x0;
      x0 = x1;
      x1 = b;
    }
    if(y0 > y1) {
      uint8_t b = y0;
      y0 = y1;
      y1 = b;
    }
    for(; x0 < x1; ++x0) {
      for(; y0 < y1; ++y0) {
        drawPixel(x0, y0, color);
      }
    }
  }

  void drawPlotPoints(const uint8_t x0, const uint8_t y0, const uint8_t x1, const uint8_t y1, const bool color) {
    drawPixel(x0 + x1, y0 + y1, color);
    drawPixel(x0 - x1, y0 + y1, color);
    drawPixel(x0 + x1, y0 - y1, color);
    drawPixel(x0 - x1, y0 - y1, color);
    drawPixel(x0 + x1, y0 + y1, color);
    drawPixel(x0 - x1, y0 + y1, color);
    drawPixel(x0 + x1, y0 - y1, color);
    drawPixel(x0 - x1, y0 - y1, color);
  }

  void drawCircle(const uint8_t x, const uint8_t y, const uint8_t radius, const bool color) {
    int8_t X = 0;
    int8_t Y = radius;
    int16_t P = 3 - 2 * radius;

    while(X < Y) {
      drawPlotPoints(x, y, X, Y, color);
      if(P < 0) {
        P = P + 4 * X + 6;
      }
      else {
        P = P + 4 * (X - Y) + 10;
        --Y;
      }
      ++X;
    }
    drawPlotPoints(x, y, X, Y, color);
  }

  void drawPlotLines(const uint8_t x0, const uint8_t y0, const uint8_t x1, const uint8_t y1, const bool color) {
    drawLine(x0 - x1, y0 + y1, x0 + x1, y0 + y1, color);
    drawLine(x0 - x1, y0 - y1, x0 + x1, y0 - y1, color);
    drawLine(x0 - y1, y0 + x1, x0 + y1, y0 + x1, color);
    drawLine(x0 - y1, y0 - x1, x0 + y1, y0 - x1, color);
  }


  void drawFillCircle(const uint8_t x, const uint8_t y, const uint8_t radius, const bool color) {
    int8_t X = 0;
    int8_t Y = radius;
    int16_t P = 3 - 2 * radius;

    while(X < Y) {
      drawPlotLines(x, y, X, Y, color);
      if(P < 0) {
        P = P + 4 * X + 6;
      }
      else {
        P = P + 4 * (X - Y) + 10;
        --Y;
      }
      ++X;
    }
    drawPlotLines(x, y, X, Y, color);
  }

  void display() {
    uint16_t k = 0;
    for(uint8_t i = 0; i < 33; ++i) {
      Wire.beginTransmission(address);
      Wire.write(0x40);
      for(uint8_t j = 0; j < 31; ++j) {
        Wire.write(buffer_[k]);
        ++k;
      }
      Wire.endTransmission();
    }
    Wire.beginTransmission(address);
    Wire.write(0x40);
    Wire.write(buffer_[k]);
    Wire.endTransmission();
  }
};