#include "LcdDriver.hpp"

LcdDriver::LcdDriver() {
  debug(F("LcdDriver()"));
}

LcdDriver::~LcdDriver() {
  debug(F("~LcdDriver()"));
}

void LcdDriver::create() {

}

void LcdDriver::destroy() {

}

#if INTERPRETER_TEXTCODE
void LcdDriver::textcode() {
  switch(strHashSwitch(interpreter->arg().c_str())) {
    case strHashSwitch("NOP"):
      break;
    case strHashSwitch("BEG"):
      lcd.begin();
      break;
    case strHashSwitch("END"):
      lcd.end();
      break;
    case strHashSwitch("ED"):
      lcd.enableDisplay(interpreter->value());
      break;
    case strHashSwitch("EB"):
      lcd.enableBacklight(interpreter->value());
      break;
    case strHashSwitch("EC"):
      lcd.enableCursor(interpreter->value());
      break;
    case strHashSwitch("EL"):
      lcd.enableBlink(interpreter->value());
      break;
    case strHashSwitch("S"):
      lcd.seekCursor(interpreter->value(), interpreter->value());
      break;
    case strHashSwitch("W"):
      lcd.write(interpreter->value());
      break;
    case strHashSwitch("P"):
      lcd.print(interpreter->str());
      break;
    default:
      interpreter->setError(Interpreter::Error::InvalidCommand);
      break;
  }
}
#endif

#if INTERPRETER_BYTECODE
void LcdDriver::bytecode() {

}
#endif