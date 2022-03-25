#include "interpreter.hpp"

#if INTERPRETER
#include "driver.hpp"
#include "UartDriver.hpp"
#include "LcdDriver.hpp"
#include "I2CDriver.hpp"
#include "Oled1Driver.hpp"
#include "switch.h"

Interpreter* interpreter = nullptr;

Interpreter::Interpreter() {
  debug(F("Interpreter()"));
}

Interpreter::~Interpreter() {
  debug(F("~Interpreter()"));
  if(drivers_) {
    for(uint8_t i = 0; i < driversCount_; ++i) {
      delete drivers_[i];
    }
    delete drivers_;
    drivers_ = nullptr;
  }
}

#if INTERPRETER_INTERACTIVE
void Interpreter::interactive() {
  string com;
  do {
    Serial.print(F("> "));
    stateLedOff();
    while(!Serial.available()) {};
    stateLedOn();
    com = Serial.readString();
    Serial.println(com);
  } while(!textcode(com));
}
#endif

#if INTERPRETER_TEXTCODE
bool Interpreter::textcode(const string command) {
  command_ = command;
  switch(strHashSwitch(arg().c_str())) {
    case strHashSwitch("NOP"):
      break;
    case strHashSwitch("BRK"):
#if SERIAL_UI
    {
      uint8_t r = 0;
      for(uint8_t i = 0; i < sizeof(registers) / sizeof(number) / 4; ++i) {
        for(uint8_t j = 0; j < 4; ++j) {
          Serial.print('r');
          Serial.print(r);
          Serial.print(':');
          Serial.print(registers[r]);
          if(j != 3)
            Serial.print('\t');
          ++r;
        }
        Serial.println();
      }

      if(driversCount_ == 0) break;
      Serial.println(F("Drivers loaded:"));
      for(uint8_t i = 0; i < driversCount_; ++i) {
        Serial.print(driversTable_[i]);
        Serial.print('\t');
        Serial.print(F("0x"));
        Serial.println(reinterpret_cast<uint32_t>(drivers_[i]), HEX);
      }
    }
#endif
    break;
    case strHashSwitch("HALT"):
      halt();
      break;
    case strHashSwitch("STOP"):
#if SERIAL_UI
      Serial.println(F("Idle user input..."));
      while(Serial.available()) { Serial.read(); }
      while(!Serial.available()) {}
      while(Serial.available()) { Serial.read(); }
#endif
      break;
    case strHashSwitch("EXIT"):
      return true;
      break;
    case strHashSwitch("INT"):
      break;
    case strHashSwitch("SLP"):
      delay(value());
      break;
    case strHashSwitch("SET"):
    {
      number to = reg();
      number from = value();
      registers[to] = from;
    }
    break;
    case strHashSwitch("SWAP"):
    {
      number a = reg();
      number b = reg();
      number c = registers[a];
      registers[a] = registers[b];
      registers[b] = c;
    }
    break;
    case strHashSwitch("SNT"):
    case strHashSwitch("SEQ"):
    case strHashSwitch("SNQ"):
    case strHashSwitch("SGE"):
    case strHashSwitch("SLE"):
    case strHashSwitch("SGT"):
    case strHashSwitch("SLT"):
      break;
    case strHashSwitch("ADD"):
    {
      number out = reg();
      number a = value();
      number b = value();
      registers[out] = a + b;
    }
    break;
    case strHashSwitch("SUB"):
    {
      number out = reg();
      number a = value();
      number b = value();
      registers[out] = a - b;
    }
    break;
    case strHashSwitch("MUL"):
    {
      number out = reg();
      number a = value();
      number b = value();
      registers[out] = a * b;
    }
    break;
    case strHashSwitch("DIV"):
    {
      number out = reg();
      number a = value();
      number b = value();
      registers[out] = a / b;
    }
    break;
    case strHashSwitch("DL"):
      driverLoad(arg());
      break;
    case strHashSwitch("DU"):
      driverUnload(arg());
      break;
    case strHashSwitch("D"):
      driverCall(arg());
      break;
    default:
      error_ = Error::InvalidCommand;
  }

  if(error_ != Error::Null) {
#if INTERPRETER_LOG
    File file("log.txt", O_APPEND | O_CREAT | O_READ);
#endif // INTERPRETER_LOG
    switch(error_) {
      case Error::Unknown:
#if SERIAL_UI
        Serial.println(F("Unknown error_:"));
        Serial.println(command_);
#endif
#if INTERPRETER_LOG
        file.write("Unknown error_:");
        file.write(command_.c_str());
#endif
        break;
      case Error::InvalidCommand:
#if SERIAL_UI
        Serial.println(F("Invalid command:"));
        Serial.println(command_);
#endif
#if INTERPRETER_LOG
        file.write("Invalid command:");
        file.write(command_.c_str());
#endif
        break;
      case Error::InvalidArgument:
#if SERIAL_UI
        Serial.println(F("Invalid argument:"));
        Serial.println(command_);
#endif
#if INTERPRETER_LOG
        file.write("Invalid argument:");
        file.write(command_.c_str());
#endif
        break;
      case Error::InvalidRegister:
#if SERIAL_UI
        Serial.println(F("Invalid register:"));
        Serial.println(command_);
#endif
#if INTERPRETER_LOG
        file.write("Invalid register:");
        file.write(command_.c_str());
#endif
        break;
      case Error::InvalidDriver:
#if SERIAL_UI
        Serial.println(F("Invalid driver:"));
        Serial.println(command_);
#endif
#if INTERPRETER_LOG
        file.write("Invalid driver:");
        file.write(command_.c_str());
#endif
        break;
      case Error::InvalidString:
#if SERIAL_UI
        Serial.println(F("Invalid string:"));
        Serial.println(command_);
#endif
#if INTERPRETER_LOG
        file.write("Invalid string:");
        file.write(command_.c_str());
#endif
        break;
    }
#if DEBUG
    for(uint8_t i = 0; i < errorPos_; ++i) {
#if SERIAL_UI
      Serial.print(' ');
#endif
#if INTERPRETER_LOG
      file.write(' ');
#endif
    }
    for(uint8_t i = 0; i < errorLen_; ++i) {
#if SERIAL_UI
      Serial.print('~');
#endif
#if INTERPRETER_LOG
      file.write('~');
#endif
    }
#if SERIAL_UI
    Serial.println();
#endif
#if INTERPRETER_LOG
    file.write('\n');
#endif
#endif
    error_ = Error::Null;
#if INTERPRETER_LOG
    file.close();
#endif
  }
#if DEBUG
  errorPos_ = 0;
  errorLen_ = 0;
#endif
  pos_ = 0;
  return false;
}
#endif

#if INTERPRETER_BYTECODE
bool Interpreter::bytecode(const string command) {
  command_ = command;
  return false;
}
#endif

#if INTERPRETER_COMPILLER
string Interpreter::compile(const string command) {
  return string();
}
#endif

void Interpreter::setError(const Error error) {
  error_ = error;
}

string Interpreter::arg() {
  string out;
  if(command_.length() == pos_) {
    error_ = Error::InvalidArgument;
    return out;
  }
  out.reserve(6);
  char s;
  do {
    s = command_[pos_];
    ++pos_;
    if(s == ' ') break;
    out += s;
  } while(command_.length() != pos_);
#if DEBUG
  Serial.print(F("ARG: "));
  Serial.println(out);
  if(error_ == Error::Null) {
    errorLen_ = out.length();
    errorPos_ = pos_ - errorLen_ - (s == ' ' ? 1 : 0);
  }
#endif // DEBUG
  return out;
}

Interpreter::number Interpreter::value() {
  string out = arg();
  if(out[0] == 'r') {
    const uint8_t reg = out.substring(1).toInt();
    if(reg > 15) {
      error_ = Error::InvalidRegister;
      return 0;
    }
#if DEBUG
    Serial.print(F("REG: "));
    Serial.println(reg);
    Serial.print(F("VAL: "));
    Serial.println(registers[reg]);
#endif // DEBUG
    return registers[reg];
  }
  else {
#if DEBUG
    number val = out.toInt();
    Serial.print(F("VAL: "));
    Serial.println(val);
    return val;
#else
    return out.toInt();
#endif // DEBUG
  }
}

Interpreter::number Interpreter::reg() {
  string out = arg();
  if(out[0] == 'r') {
    const uint8_t reg = out.substring(1).toInt();
    if(reg > ASM_REGISTER_COUNT) {
      error_ = Error::InvalidRegister;
      return 0;
    }
#if DEBUG
    Serial.print(F("REG: "));
    Serial.println(reg);
#endif // DEBUG
    return reg;
  }
  error_ = Error::InvalidRegister;
  return 0;
}

string Interpreter::str() {
  string out = arg();
  if(out[0] == '\"' && out[out.length() - 1] == '\"') {
#if DEBUG && SERIAL_UI
    Serial.print(F("STR: "));
    Serial.println(out);
#endif // DEBUG && SERIAL_UI
    out = out.substring(1, out.length() - 1);
  }
  else {
    error_ = Error::InvalidString;
  }
  return out;
}

uint8_t Interpreter::driverAddr(const string name) {
  switch(strHashSwitch(name.c_str())) {
    case strHashSwitch("UART"):
      return 1;
    case strHashSwitch("LCD"):
      return 2;
    case strHashSwitch("I2C"):
      return 3;
    case strHashSwitch("OLED1"):
      return 4;
  }
  return 255;
}

void Interpreter::driverLoad(const string name) {
  const uint8_t addr = driverAddr(name);
  if(addr == 255) {
    error_ = Error::InvalidDriver;
    return;
  }

  if(driversCount_ != 0) {
    for(uint8_t i = 0; i < driversCount_; ++i) {
      if(driversTable_[i] == addr) {
        return;
      }
    }
  }

  drivers_ = reinterpret_cast<Driver**>(realloc(drivers_, (++driversCount_) * sizeof(Driver*)));
  driversTable_ = reinterpret_cast<uint8_t*>(realloc(driversTable_, driversCount_));

  const uint8_t index = driversCount_ - 1;
  switch(addr) {
    case 1:
      drivers_[index] = new UartDriver;
      break;
    case 2:
      drivers_[index] = new LcdDriver;
      break;
    case 3:
      drivers_[index] = new I2CDriver;
      break;
    case 4:
      drivers_[index] = new Oled1Driver;
      break;
  }
  drivers_[index]->create();
  driversTable_[index] = addr;
}

void Interpreter::driverUnload(const string name) {
  const uint8_t addr = driverAddr(name);

  uint8_t i;
  if(driversCount_ != 0) {
    for(i = 0; i < driversCount_; ++i) {
      if(driversTable_[i] == addr) {
        break;
      }
    }
  }
  delete drivers_[i];

  if(i == driversCount_ - 1) {
    return;
  }

  for(uint8_t j = i; j < driversCount_ - 1; ++j) {
    drivers_[j] = drivers_[j + 1];
    driversTable_[j] = driversTable_[j + 1];
  }

  drivers_ = reinterpret_cast<Driver**>(realloc(drivers_, (--driversCount_) * sizeof(Driver*)));
  driversTable_ = reinterpret_cast<uint8_t*>(realloc(driversTable_, driversCount_));
}

void Interpreter::driverCall(const string name) {
  if(!drivers_) return;

  bool found = false;
  uint8_t addr = driverAddr(name);
  for(uint8_t i = 0; i < driversCount_; ++i) {
    if(driversTable_[i] == addr) {
      addr = i;
      found = true;
      break;
    }
  }
  if(found) {
    drivers_[addr]->textcode();
  }
  else {
    error_ = Error::InvalidDriver;
  }
}
#endif