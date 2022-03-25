#pragma once

#include "OledI2c.hpp"
#include "driver.hpp"

class Oled1Driver : public Driver {
  OledI2c<0x3C> oled;
public:

  Oled1Driver();

  ~Oled1Driver();

  virtual void create() override;

  virtual void destroy() override;

#if INTERPRETER_TEXTCODE
  virtual void textcode() override;
#endif

#if INTERPRETER_BYTECODE
  virtual void bytecode() override;
#endif
};