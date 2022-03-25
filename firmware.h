#pragma once

#include "asm.hpp"

#define ENABLE true
#define DISABLE false

#ifdef BUILTIN_LED

#undef BUILTIN_LED

#endif // BUILTIN_LED

#ifdef LED_BUILTIN

#define BUILTIN_LED LED_BUILTIN

#else

#define BUILTIN_LED 13

#endif // LED_BUILTIN

#ifndef BUILTIN_SDCARD

#define BUILTIN_SDCARD 33

#endif // BUILTIN_SDCARD

//------------------------------ SETTINGS ---------------------------------
#define DEBUG                     ENABLE 

#define SERIAL_UI                 ENABLE
#define SERIAL_BAUDRATE           115200
#define SERIAL_FULL_UI            ENABLE

#define LED_STATE                 ENABLE
#define LED_STATE_INVERT          ENABLE
#define PIN_STATE_LED             BUILTIN_LED
#define PIN_SD_CS                 BUILTIN_SDCARD

#define EXPLORER                  ENABLE
#define EXPLORER_FORTUNE_UTIL     ENABLE
#define EXPLORER_FULL_OUTPUT      ENABLE

#define INTERPRETER               ENABLE
#define INTERPRETER_INTERACTIVE   ENABLE
#define INTERPRETER_TEXTCODE      ENABLE
#define INTERPRETER_BYTECODE      ENABLE
#define INTERPRETER_COMPILLER     ENABLE
#define INTERPRETER_LOG           ENABLE

#define ASM_REGISTER_COUNT        16
#define ASM_PAGE_SIZE             1024
//-------------------------------------------------------------------------

#if SERIAL_FULL_UI && SERIAL_UI
#define fprint(str)   Serial.print(str)
#define fprintln(str) Serial.println(str)
#else  // SERIAL_FULL_UI
#define fprint(str)
#define fprintln(str)
#endif // SERIAL_FULL_UI

#if DEBUG && SERIAL_UI
#define debug(str) Serial.println(str)
#else  // DEBUG
#define debug(str)
#endif // DEBUG

#if LED_STATE

#if LED_STATE_INVERT
#define stateLedOn()  digitalWrite(PIN_STATE_LED, LOW)
#define stateLedOff() digitalWrite(PIN_STATE_LED, HIGH)
#else  // LED_STATE_INVERT
#define stateLedOn()  digitalWrite(PIN_STATE_LED, HIGH)
#define stateLedOff() digitalWrite(PIN_STATE_LED, LOW)
#endif // LED_STATE_INVERT

#else  // LED_STATE

#define stateLedOn()
#define stateLedOff()

#endif // LED_STATE