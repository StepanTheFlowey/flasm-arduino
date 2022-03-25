#include "Oled.hpp"

Oled::Oled() {
  debug(F("Oled()"));
}

Oled::~Oled() {
  debug(F("~Oled()"));
}

void Oled::begin() {
  buffer_ = new uint8_t[1024];
  sendSequence(SSD1306_InitSequence, sizeof(SSD1306_InitSequence));
}

void Oled::end() {
  if(buffer_) {
    delete buffer_;
    buffer_ = nullptr;
  }
}

void Oled::sendSingle(const uint8_t command) {}

void Oled::sendSequence(const uint8_t* sequence, const uint8_t lenght) {}