#pragma once

#include "Driver.hpp"

#include <Stream.h>

class UartDriver : public Driver {
  static Stream* serialTable[];
  uint8_t serialNumber = 0;
public:

  UartDriver();

  ~UartDriver();

  virtual void create() override;

  virtual void destroy() override;

  virtual void textcode() override;

  virtual void bytecode() override;
private:

  void callBegin(unsigned long baudrate);

  void callEnd();

  Stream* underlying();
};