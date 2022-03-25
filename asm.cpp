#include "asm.hpp"

#include "firmware.h"
#include "hardware.h"

#include "explorer.hpp"
#include "interpreter.hpp"

Sd sd;

void halt() {
  stateLedOff();
#ifdef BOARD_ESP
  disableCore0WDT();
  disableCore1WDT();

  while(true) __asm__ volatile("nop");
#else
  exit(0);
#endif
}

void shutdown() {
#if INTERPRETER
  delete interpreter;
#endif
#if EXPLORER
  delete explorer;
#endif
#if SERIAL_FULL_UI
  Serial.println(F("System is shutdown"));
#endif // SERIAL_FULL_UI
  Serial.end();
  sd.end();

  delay(1000);

  halt();
}

void setup() {
  pinMode(PIN_STATE_LED, OUTPUT);
  stateLedOn();

#if SERIAL_UI
  Serial.begin(SERIAL_BAUDRATE);
  Serial.setTimeout(100);
  while(!Serial) {};
  Serial.println(F("Welcome to Flowey`s ASM!"));
#endif //SERIAL_UI

#if EXPLORER
  explorer = new Explorer;
  Explorer::uname();
  Explorer::init();
#endif // EXPLORER

#if INTERPRETER
  interpreter = new Interpreter;
  interpreter->textcode("BRK");
#endif
}

void loop() {
  fprintln(F("Select option:"));
  Serial.println(F("0. Shutdown"));
  Serial.println(F("1. Explorer"));
  Serial.println(F("2. Interpretter"));
  Serial.print(F("# "));
  Serial.flush();

  stateLedOff();
  while(!Serial.available()) {}
  stateLedOn();

  uint8_t choice = Serial.read();
  Serial.write(choice);
  while(Serial.available()) { Serial.read(); }
  Serial.println();

  switch(choice) {
    case '0':
      shutdown();
      break;
    case '1':
      fprintln(F("Starting explorer..."));
      explorer->interactive();
      break;
    case '2':
      fprintln(F("Starting interpreter..."));
      interpreter->interactive();
      break;
    default:
      fprintln(F("Unknown option!"));
      break;
  }
}