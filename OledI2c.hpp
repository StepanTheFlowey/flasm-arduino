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

  virtual void display() override {
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