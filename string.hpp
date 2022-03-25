/*
  WString.h - String library for Wiring & Arduino
  ...mostly rewritten by Paul Stoffregen...
  Copyright (c) 2009-10 Hernando Barragan.  All right reserved.
  Copyright 2011, Paul Stoffregen, paul@pjrc.com

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

// When compiling programs with this class, the following gcc parameters
// dramatically increase performance and memory (RAM) efficiency, typically
// with little or no increase in code size.
//     -felide-constructors
//     -std=c++0x

#pragma once

#ifdef __INTELLISENSE__
#include "asm.hpp"
#endif //__INTELLISENSE__

#include <Print.h>
#include <Printable.h>
#include <WString.h>

// An inherited class for holding the result of a concatenation.  These
// result objects are assumed to be writable by subsequent concatenations.
class stringSumHelper;

// The string class
class string : public Printable {
  // use a function pointer to allow for "if (s)" without the
  // complications of an operator bool(). for more information, see:
  // http://www.artima.com/cppsource/safebool.html
  typedef void (string::* StringIfHelperType)() const;
  void StringIfHelper() const {}

public:
  // constructors
  // creates a copy of the initial value.
  // if the initial value is null or invalid, or if memory allocation
  // fails, the string will be marked as invalid (i.e. "if (s)" will
  // be false).
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
  string(string&& rval);
  string(stringSumHelper&& rval);
#endif
  explicit string(char c);
  explicit string(int, uint8_t base = 10);
  explicit string(long, uint8_t base = 10);
  explicit string(unsigned char, uint8_t base = 10);
  explicit string(unsigned int, uint8_t base = 10);
  explicit string(unsigned long, uint8_t base = 10);
  explicit string(float, uint8_t decimalPlaces = 2);
  explicit string(double, uint8_t decimalPlaces = 2);
  string(const char* cstr = "");
  string(const string& str);
  string(const String& str);
  string(const __FlashStringHelper* str);
  ~string();

  virtual size_t printTo(Print& p) const override {
    return p.write(buffer, len);
  }

  // memory management
  // return true on success, false on failure (in which case, the string
  // is left unchanged).  reserve(0), if successful, will validate an
  // invalid string (i.e., "if (s)" will be true afterwards)
  uint8_t reserve(unsigned int size);
  inline unsigned int length(void) const { return len; }
  inline unsigned int size(void) const { return len; }

  // creates a copy of the assigned value.  if the value is null or
  // invalid, or if the memory allocation fails, the string will be
  // marked as invalid ("if (s)" will be false).
  string& operator=(const char* cstr);
  string& operator=(const string& rhs);
  string& operator=(const String& str);
  string& operator=(const __FlashStringHelper* str);
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
  string& operator=(string&& rval);
  string& operator=(stringSumHelper&& rval);
#endif

  // concatenate (works w/ built-in types)

  // returns true on success, false on failure (in which case, the string
  // is left unchanged).  if the argument is null or invalid, the
  // concatenation is considered unsucessful.
  uint8_t concat(char c);
  uint8_t concat(int num);
  uint8_t concat(long num);
  uint8_t concat(unsigned char c);
  uint8_t concat(unsigned int num);
  uint8_t concat(unsigned long num);
  uint8_t concat(float num);
  uint8_t concat(double num);
  uint8_t concat(const char* cstr);
  uint8_t concat(const string& str);
  uint8_t concat(const String& str);
  uint8_t concat(const __FlashStringHelper* fstr);

  // if there's not enough memory for the concatenated value, the string
  // will be left unchanged (but this isn't signalled in any way)
  inline string& operator+=(char c) { concat(c); return (*this); }
  inline string& operator+=(int num) { concat(num); return (*this); }
  inline string& operator+=(long num) { concat(num); return (*this); }
  inline string& operator+=(unsigned char num) { concat(num); return (*this); }
  inline string& operator+=(unsigned int num) { concat(num); return (*this); }
  inline string& operator+=(unsigned long num) { concat(num); return (*this); }
  inline string& operator+=(float num) { concat(num); return (*this); }
  inline string& operator+=(double num) { concat(num); return (*this); }
  inline string& operator+=(const char* cstr) { concat(cstr); return (*this); }
  inline string& operator+=(const string& rhs) { concat(rhs); return (*this); }
  inline string& operator+=(const String& str) { concat(str); return (*this); }
  inline string& operator+=(const __FlashStringHelper* str) { concat(str); return (*this); }

  friend stringSumHelper& operator+(const stringSumHelper& lhs, char c);
  friend stringSumHelper& operator+(const stringSumHelper& lhs, int num);
  friend stringSumHelper& operator+(const stringSumHelper& lhs, long num);
  friend stringSumHelper& operator+(const stringSumHelper& lhs, unsigned char num);
  friend stringSumHelper& operator+(const stringSumHelper& lhs, unsigned int num);
  friend stringSumHelper& operator+(const stringSumHelper& lhs, unsigned long num);
  friend stringSumHelper& operator+(const stringSumHelper& lhs, float num);
  friend stringSumHelper& operator+(const stringSumHelper& lhs, double num);
  friend stringSumHelper& operator+(const stringSumHelper& lhs, const char* cstr);
  friend stringSumHelper& operator+(const stringSumHelper& lhs, const string& rhs);
  friend stringSumHelper& operator+(const stringSumHelper& lhs, const String& rhs);
  friend stringSumHelper& operator+(const stringSumHelper& lhs, const __FlashStringHelper* rhs);

  // comparison (only works w/ Strings and "strings")
  operator StringIfHelperType() const { return buffer ? &string::StringIfHelper : 0; }
  int compareTo(const string& s) const;
  bool equals(const string& s) const;
  bool equals(const char* cstr) const;
  bool operator==(const char ch) const;
  bool operator==(const string& rhs) const { return equals(rhs); }
  bool operator==(const char* cstr) const { return equals(cstr); }
  bool operator!=(const string& rhs) const { return !equals(rhs); }
  bool operator!=(const char* cstr) const { return !equals(cstr); }
  bool operator< (const string& rhs) const;
  bool operator> (const string& rhs) const;
  bool operator<=(const string& rhs) const;
  bool operator>=(const string& rhs) const;
  uint8_t equalsIgnoreCase(const string& s) const;
  bool startsWith(const char c) const;
  bool startsWith(const string& prefix) const;
  bool startsWith(const string& prefix, unsigned int offset) const;
  bool endsWith(const char c) const;
  bool endsWith(const string& suffix) const;

  // character acccess
  char charAt(unsigned int index) const;
  void setCharAt(unsigned int index, char c);
  char  operator[](unsigned int index) const;
  char& operator[](unsigned int index);
  void getBytes(unsigned char* buf, unsigned int bufsize, unsigned int index = 0) const;
  void toCharArray(char* buf, unsigned int bufsize, unsigned int index = 0) const;
  const char* c_str() const { return buffer; }
  char* begin() { return buffer; }
  char* end() { return buffer + length(); }
  const char* begin() const { return c_str(); }
  const char* end() const { return c_str() + length(); }

  // search
  int indexOf(char ch) const;
  int indexOf(char ch, unsigned int fromIndex) const;
  int indexOf(const string& str) const;
  int indexOf(const string& str, unsigned int fromIndex) const;
  int lastIndexOf(char ch) const;
  int lastIndexOf(char ch, unsigned int fromIndex) const;
  int lastIndexOf(const string& str) const;
  int lastIndexOf(const string& str, unsigned int fromIndex) const;
  string substring(unsigned int beginIndex) const { return substring(beginIndex, len); };
  string substring(unsigned int beginIndex, unsigned int endIndex) const;

  // modification
  void replace(char find, char replace);
  void replace(const string& find, const string& replace);
  void remove(unsigned int index);
  void remove(unsigned int index, unsigned int count);
  void toLowerCase(void);
  void toUpperCase(void);
  void trim(void);

  // parsing/conversion
  int toInt(void) const;
  long toLong(void) const;
  float toFloat(void) const;
  double toDouble(void) const;
protected:

  char* buffer;	          // the actual char array
  unsigned int capacity;  // the array length minus one (for the '\0')
  unsigned int len;       // the string length (not counting the '\0')

  void init(void);
  void invalidate(void);
  uint8_t changeBuffer(unsigned int maxStrLen);
  uint8_t concat(const char* cstr, unsigned int length);

  // copy and move
  string& copy(const char* cstr, unsigned int length);
  string& copy(const __FlashStringHelper* fstr, unsigned int length);
#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
  void move(string& rhs);
#endif
};

class stringSumHelper : public string {
public:

  inline stringSumHelper(char c) : string(c) {}
  inline stringSumHelper(int num) : string(num) {}
  inline stringSumHelper(long num) : string(num) {}
  inline stringSumHelper(unsigned char num) : string(num) {}
  inline stringSumHelper(unsigned int num) : string(num) {}
  inline stringSumHelper(unsigned long num) : string(num) {}
  inline stringSumHelper(float num) : string(num) {}
  inline stringSumHelper(double num) : string(num) {}
  inline stringSumHelper(const char* p) : string(p) {}
  inline stringSumHelper(const string& s) : string(s) {}
  inline stringSumHelper(const __FlashStringHelper* fstr) : string(fstr) {}
};