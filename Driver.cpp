#include "driver.hpp"

Driver::Driver() {
  debug(F("Driver()"));
}

Driver::~Driver() {
  debug(F("~Driver()"));
}

void Driver::create() {
  debug(F("Driver::create()!"));
}

void Driver::destroy() {
  debug(F("Driver::destroy()!"));
}

void Driver::textcode() {
  debug(F("Driver::textcode()!"));
}

void Driver::bytecode() {
  debug(F("Driver::bytecode()!"));
}