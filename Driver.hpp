#pragma once

#include "interpreter.hpp"

class Driver {
public:

  Driver();

  virtual ~Driver();

  virtual void create();

  virtual void destroy();

#if INTERPRETER_TEXTCODE
  virtual void textcode();
#endif

#if INTERPRETER_BYTECODE
  virtual void bytecode();
#endif
};