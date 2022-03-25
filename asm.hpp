//-------------------- Flowey ASM ----------------------
//Features:
// ASM:
//  General:
//   OP   Syntax                             OPID DONE  Description
//   ---- BASE ---------------------------------------------------
//   NOP                                     0    true  Idle clock
//   BRK                                     1    true  Debug
//   HALT                                    2    true  Forces interpreter to skip all
//   STOP                                    3    true  Idle user
//   EXIT                                    3    true  Exit program
//   INT  <num>                              4    true  Calls interrupt
//   SLP  <delay>                            5    true  Sleeps milliseconds
//   ---- REGISTERS -----------------------------------
//   SET  <reg to> <from>                    4    true  Sets register to value
//   SWAP <reg first> <reg second>           5    true  Swaps registers
//   ---- BASIC LOGIC ---------------------------------
//   SNT  <reg out> <a>                      6    false Set NoT
//   SEQ  <reg out> <a> <b>                  7    false Set EQual
//   SNQ  <reg out> <a> <b>                  8    false Set Not eQual
//   SGE  <reg out> <a> <b>                  9    false Set Greater Equal
//   SLE  <reg out> <a> <b>                  10   false Set Less Equal
//   SGT  <reg out> <a> <b>                  11   false Set Greater Than
//   SLT  <reg out> <a> <b>                  12   false Set Less Than
//   ---- BASIC MATH ----------------------------------
//   ADD  <reg out> <a> <b>                  13   true  Adds numbers
//   SUB  <reg out> <a> <b>                  14   true  Substracts numbers
//   MUL  <reg out> <a> <b>                  15   true  Multiplys numbers
//   DIV  <reg out> <a> <b>                  16   true  Divides numbers
//   ---- STACK ---------------------------------------
//   PUSH <num>                              17   false Pushs number to stack
//   POP  <reg out>                          18   false Pops number from stack to register
//   ---- MEMORY --------------------------------------
//   LD   <reg in> <address>                 19   false Loads value from page
//   ST   <address> <reg out>                20   false Stores value to page
//   ---- BRANCH --------------------------------------
//   JUMP <line>                             21   false Jumps to line
//   CALL <func>                             22   false Calls function
// Advenced:
//   ---- GPIO ----------------------------------------
//   GPM  <pin> <mode>                       24   false Gpio Pin Mode
//   GDR  <reg out> <pin>                    25   false Gpio Digital Read
//   GDW  <pin> <value>                      24   false Gpio Digital Write
//   GAR  <reg out> <pin>                    24   false Gpio Analog Read
//   GAW  <pin> <value>                      24   false Gpio Analog Write
//   GARR <value>                            24   false Gpio Analog Read Resolution
//   GAWR <value>                            24   false Gpio Analog Write Resolution
//   ---- PAGES ---------------------------------------
//   PA   <number>                           23   false Page Alloc
//   PL   <number>                           24   false Page Load
//   PD   <number>                           25   false Page Unload
//   ---- DRIVERS -------------------------------------
//   DL   <driver name>                      23   false Driver Load
//   DU   <driver name>                      23   false Driver Unload
//   D    <driver name> <command> [args]     23   false Driver call
// Drivers:
//  UART:
//   NOP                                     0     true Idle clock
//   SEL  <uart id>                          1     true Selects serial to use
//   BEG  <baudrate>                         1     true begin(baudrate)
//   END                                     2     true end()
//   A    <reg out>                          3     true available()
//   R    <reg out>                          3     true read()
//   W    <num>                              2     true write(num)
//   P    <val>                              1     true print(val)
//  I2C:
//   NOP                                     0     true Idle clock
//   BEG                                     0     true begin()
//   SLV <val>                               0     true begin(val)
//   END                                     0     true end()
//   CLK <val>                               0     true setClock()
//  LCD:
//   NOP                                     0     true Idle clock
//   BEG                                     1     true begin()
//   END                                     2     true end()
//   CLS                                     5     true Clear Lcd Screen
//   ED   <val>                              3     true Enable Display
//   EB   <val>                              3     true Enable Backlight
//   EC   <val>                              3     true Enable Cursor
//   EL   <val>                              4     true Enable bLink
//   S    <x> <y>                            5     true seek(x, y)
//   W    <num>                              4     true write(num)
//   P    <val>                              3     true print(val)
//  OLED1:
//   NOP                                     0     true Idle clock
//   BEG                                     1     true begin()
//   END                                     2     true end()
//   CLS                                     5     true Clear Lcd Screen
//   DP   <x> <y> <val>                      6     true Draw Point
//   DL   <x0> <y0> <x1> <x1> <val>          7     true Draw Line
//   DR   <x0> <y0> <x1> <x1> <val>          7     true Draw Rect
//   DFR  <x0> <y0> <x1> <y1> <val>          8     true Draw Fill Rect
//   DC   <x0> <y0> <radius> <val>           7     true Draw Circle
//   DFC  <x0> <y0> <radius> <val>           8     true Draw Fill Circle
//   D                                       9     true Display
//   
//
// Explorer:
//  list   - ls - Lists directores to serial     - true
//  mkdir  - md - Creates directory              - true
//  rmdir  - rd - Removes directory              - true
//  chdir  - cd - Changes current directory      - true
//  remove - rm - Removes file                   - true
//  move   - mv - Moves or renames file          - false
//  touch  - __ - Creates file                   - true
//  tee    - __ - Adds content to file           - true
//  cat    - __ - Output file to serial          - true
//  edit   - __ - Interactive editor             - false
//  uname  - um - Displays target hardware info  - true
// Interpretter:
//  
#pragma once

#define Sd __Sd
#define File __File

#ifdef __INTELLISENSE__ 

#if 0

#define USB_SERIAL
#define F_CPU 180000000
#define __MK66FX1M0__ 1

#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\Arduino.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\arm_common_tables.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\arm_math.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\AudioStream.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\avr_emulation.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\avr_functions.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\binary.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\Client.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\core_cm4.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\core_cm4_simd.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\core_cmInstr.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\core_id.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\core_pins.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\CrashReport.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\DMAChannel.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\elapsedMillis.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\EventResponder.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\FS.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\HardwareSerial.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\IntervalTimer.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\IPAddress.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\Keyboard.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\keylayouts.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\kinetis.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\math_helper.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\MIDIUSB.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\mk20dx128.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\Mouse.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\new.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\pgmspace.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\pins_arduino.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\Print.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\Printable.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\Server.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\ser_print.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\SPIFIFO.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\Stream.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\Udp.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_audio.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_desc.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_dev.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_flightsim.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_joystick.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_keyboard.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_mem.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_midi.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_mouse.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_mtp.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_names.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_rawhid.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_seremu.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_serial.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_serial2.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_serial3.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_touch.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\usb_undef.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\WCharacter.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\WConstants.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\wiring.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\wiring_private.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\WProgram.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\WString.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\util\atomic.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\util\crc16.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\util\delay.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\teensy3\util\parity.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\Wire\Wire.h"
#include "C:\Program Files (x86)\Arduino\hardware\teensy\avr\libraries\SPI\SPI.h"

#else 

#define F_CPU 180000000
#define ESP_PLATFORM

#include "C:\Users\Flowey\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.2\cores\esp32\Arduino.h"
#include "C:\Users\Flowey\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.2\cores\esp32\HardwareSerial.h"
#include "C:\Users\Flowey\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.2\cores\esp32\WCharacter.h"
#include "C:\Users\Flowey\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.2\cores\esp32\WString.h"
#include "C:\Users\Flowey\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.2\cores\esp32\Print.h"
#include "C:\Users\Flowey\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.2\cores\esp32\Printable.h"
#include "C:\Users\Flowey\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.2\cores\esp32\Esp.h"
#include "C:\Users\Flowey\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.2\libraries\Wire\src\Wire.h"
#include "C:\Users\Flowey\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.2\libraries\SPI\src\SPI.h"

#endif

#include "C:\Users\Flowey\Documents\Arduino\libraries\SdFat\src\SdFat.h"

#endif

#include <inttypes.h>
#include <SdFat.h>

#undef Sd
#undef File

typedef SdFat32 Sd;
typedef FatFile File;

extern Sd sd;

void halt();

void shutdown();