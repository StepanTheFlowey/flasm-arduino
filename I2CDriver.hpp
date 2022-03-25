#pragma once

#include "driver.hpp"

#include <Wire.h>

class I2CDriver : public Driver {
public:

  I2CDriver();

  ~I2CDriver();

  virtual void create() override;

  virtual void destroy() override;

#if INTERPRETER_TEXTCODE
  virtual void textcode() override;
#endif

#if INTERPRETER_BYTECODE
  virtual void bytecode() override;
#endif
};