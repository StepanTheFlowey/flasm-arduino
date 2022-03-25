#include "I2CDriver.hpp"

I2CDriver::I2CDriver() {
  debug(F("I2CDriver()"));
}

I2CDriver::~I2CDriver() {
  debug(F("~I2CDriver()"));
}

void I2CDriver::create() {

}

void I2CDriver::destroy() {

}

#if INTERPRETER_TEXTCODE
void I2CDriver::textcode() {
  switch(strHashSwitch(interpreter->arg().c_str())) {
    case strHashSwitch("NOP"):
      break;
    case strHashSwitch("BEG"):
      Wire.begin();
      break;
    case strHashSwitch("SLV"):
      Wire.begin(interpreter->value());
      break;
    case strHashSwitch("END"):
      Wire.end();
      break;
    case strHashSwitch("CLK"):
      Wire.setClock(interpreter->value());
      break;
    default:
      interpreter->setError(Interpreter::Error::InvalidCommand);
      break;
  }
}
#endif

#if INTERPRETER_BYTECODE
void I2CDriver::bytecode() {

}
#endif