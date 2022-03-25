#pragma once

#include "driver.hpp"

#include "Lcd_I2C.hpp"

class LcdDriver : public Driver {
  LcdI2c<0x38, 20, 4> lcd;
public:

  LcdDriver();

  ~LcdDriver();

  virtual void create() override;

  virtual void destroy() override;

#if INTERPRETER_TEXTCODE
  virtual void textcode() override;
#endif

#if INTERPRETER_BYTECODE
  virtual void bytecode() override;
#endif
};