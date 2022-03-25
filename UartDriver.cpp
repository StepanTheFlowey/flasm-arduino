#include "UartDriver.hpp"

Stream* UartDriver::serialTable[] = {
  reinterpret_cast<Stream*>(&Serial)
};

UartDriver::UartDriver() {
  debug(F("UartDriver()"));
}

UartDriver::~UartDriver() {
  debug(F("~UartDriver()"));
}

void UartDriver::create() {

}

void UartDriver::destroy() {

}

void UartDriver::textcode() {
  switch(strHashSwitch(interpreter->arg().c_str())) {
    case strHashSwitch("NOP"):
      break;
    case strHashSwitch("SEL"):
      serialNumber = interpreter->value();
      break;
    case strHashSwitch("BEG"):
      callBegin(interpreter->value());
      break;
    case strHashSwitch("END"):
      callEnd();
      break;
    case strHashSwitch("A"):
      interpreter->registers[interpreter->reg()] = underlying()->available();
      break;
    case strHashSwitch("R"):
      interpreter->registers[interpreter->reg()] = underlying()->read();
      break;
    case strHashSwitch("W"):
      underlying()->write(interpreter->value());
      break;
    case strHashSwitch("P"):
      underlying()->print(interpreter->str());
      break;
    default:
      interpreter->setError(Interpreter::Error::InvalidCommand);
      break;
  }
}

#ifdef INTERPRETTER_BYTECODE
void UartDriver::bytecode() {

}
#endif

void UartDriver::callBegin(unsigned long baudrate) {
  switch(serialNumber) {
    case 0:
      Serial.begin(baudrate);
      break;
  }
}

void UartDriver::callEnd() {
  switch(serialNumber) {
    case 0:
      Serial.end();
      break;
  }
}

Stream* UartDriver::underlying() {
  return serialTable[serialNumber];
}