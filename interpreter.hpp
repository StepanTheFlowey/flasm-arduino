#pragma once

#include "firmware.h"

#if INTERPRETER

#include "string.hpp"
#include "switch.h"

class Driver;

class Interpreter {
public:
  
  enum class Error {
    Null,
    Unknown,
    InvalidCommand,
    InvalidArgument,
    InvalidRegister,
    InvalidDriver,
    InvalidString
  };
  using number = int16_t;
  using opcode = uint8_t;
private:

  string command_;
#ifdef DEBUG
  uint8_t errorPos_ = 0;
  uint8_t errorLen_ = 0;
#endif // DEBUG
  uint8_t pos_ = 0;
  Error error_ = Error::Null;
  Driver** drivers_ = nullptr;
  uint8_t* driversTable_ = nullptr;
  uint8_t driversCount_ = 0;
public:

  number registers[ASM_REGISTER_COUNT]{};

  Interpreter();

  ~Interpreter();

#if INTERPRETER_INTERACTIVE
  void interactive();
#endif

#if INTERPRETER_TEXTCODE
  bool textcode(const string command);
#endif

#if INTERPRETER_BYTECODE
  bool bytecode(const string command);
#endif

#if INTERPRETER_COMPILLER
  string compile(const string command);
#endif // INTERPRETTER_COMPILLER

  void setError(const Error error);

  string arg();
  number value();
  number reg();
  string str();
private:

  uint8_t driverAddr(const string name);
  void driverLoad(const string name);
  void driverUnload(const string name);
  void driverCall(const string name);
};
extern Interpreter* interpreter;
#endif