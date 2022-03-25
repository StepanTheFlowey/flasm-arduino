#pragma once

#include "firmware.h"

template <uint8_t csPin, uint8_t resetPin>
class TftSpi {
public:

  TftSpi() {
    debug(F("TftSpi"));
  }

  ~TftSpi() {
    debug(F("~TftSpi"));
  }

  void begin() {

  }

  void end() {

  }
};