#pragma once

#include "asm.hpp"

#ifdef ARDUINO_ARCH_AVR

#define ARCH_AVR
#define ARCH_NAME F("AVR")

#elif defined(ARDUINO_ARCH_SAM)

#define ARCH_SAM
#define ARCH_NAME F("SAM")

#elif defined(ESP_PLATFORM) 

#define ARCH_ARM
#define ARCH_NAME F("ARM")

#else

#define ARCH_UNKNOWN
#define ARCH_NAME F("Unknown")
#warning Unsupported archetecture

#endif //ARDUINO_ARCH_AVR

#if defined(ARDUINO_AVR_NANO)

#define BOARD_ARDUINO_NANO

#elif defined(ARDUINO_SAM_DUE)

#define BOARD_ARDUINO_DUE

#elif defined(__MK20DX128__)

#define BOARD_TEENSY_3_0
#define BOARD_NAME F("Teensy 3.0")
#define MCU_NAME F("MK20DX128")

#elif defined(__MK20DX256__)

#define BOARD_TEENSY_3_1
#define BOARD_TEENSY_3_2
#define BOARD_NAME F("Teensy 3.1 & 3.2")
#define MCU_NAME F("MK20DX256")

#elif defined(__MK66FX512__) //Not verified

#define BOARD_TEENSY_3_5
#define BOARD_NAME F("Teensy 3.5")
#define MCU_NAME F("MK66FX512")

#elif defined(__MK66FX1M0__)

#define BOARD_TEENSY_3_6
#define BOARD_NAME F("Teensy 3.6")
#define MCU_NAME F("MK66FX1M0")

#elif defined(ESP_PLATFORM)

#define BOARD_ESP
#define BOARD_NAME F("ESP")
#define MCU_NAME F("Unknown")

#define I2C_BUFFER_LENGTH 32

#else

#define BOARD_UNKNOWN
#define BOARD_NAME F("Unknown")
#define MCU_NAME F("Unknown")
#warning Unsupported MCU

#endif

#define MCU_FREQ F_CPU