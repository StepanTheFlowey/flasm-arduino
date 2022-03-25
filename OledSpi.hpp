#pragma once

#include <SPI.h>

template <uint8_t csPin>
class OledSpi {
public:

  OledSpi() {
    debug("OledSpi()");
  }

  ~OledSpi() {
    debug("~OledSpi()");
  }

  void begin() {

  }

  void end() {

  }
};