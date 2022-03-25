#include "Oled1Driver.hpp"

Oled1Driver::Oled1Driver() {
  debug(F("Oled1Driver()"));
}

Oled1Driver::~Oled1Driver() {
  debug(F("~Oled1Driver()"));
}

void Oled1Driver::create() {

}

void Oled1Driver::destroy() {

}

#if INTERPRETER_TEXTCODE
void Oled1Driver::textcode() {
  switch(strHashSwitch(interpreter->arg().c_str())) {
    case strHashSwitch("NOP"):
      break;
    case strHashSwitch("BEG"):
      oled.begin();
      break;
    case strHashSwitch("END"):
      oled.end();
      break;
    case strHashSwitch("CLS"):
      oled.clear();
      break;
    case strHashSwitch("DP"):
      oled.drawPixel(interpreter->value(),
                     interpreter->value(),
                     interpreter->value());
      break;
    case strHashSwitch("DL"):
      oled.drawLine(interpreter->value(),
                    interpreter->value(),
                    interpreter->value(),
                    interpreter->value(),
                    interpreter->value());
      break;
    case strHashSwitch("DR"):
      oled.drawRect(interpreter->value(),
                    interpreter->value(),
                    interpreter->value(),
                    interpreter->value(),
                    interpreter->value());
      break;
    case strHashSwitch("DFR"):
      oled.drawFillRect(interpreter->value(),
                        interpreter->value(),
                        interpreter->value(),
                        interpreter->value(),
                        interpreter->value());
      break;
    case strHashSwitch("DC"):
      oled.drawCircle(interpreter->value(),
                      interpreter->value(),
                      interpreter->value(),
                      interpreter->value());
      break;
    case strHashSwitch("DFC"):
      oled.drawFillCircle(interpreter->value(),
                          interpreter->value(),
                          interpreter->value(),
                          interpreter->value());
      break;
    case strHashSwitch("D"):
      oled.display();
      break;
    default:
      interpreter->setError(Interpreter::Error::InvalidCommand);
      break;
  }
}
#endif

#if INTERPRETER_BYTECODE
void Oled1Driver::bytecode() {

}
#endif