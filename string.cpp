/*
  WString.cpp - String library for Wiring & Arduino
  ...mostly rewritten by Paul Stoffregen...
  Copyright (c) 2009-10 Hernando Barragan.  All rights reserved.
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
#include "string.hpp"

#include <stdlib_noniso.h>

string::string(const char* cstr) {
  init();
  if(cstr) copy(cstr, strlen(cstr));
}

string::string(const string& value) {
  init();
  operator=(value);
}

string::string(const String& str) {
  init();
  operator=(str);
}

string::string(const __FlashStringHelper* fstr) {
  init();
  operator=(fstr);
}

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
string::string(string&& rval) {
  init();
  move(rval);
}
string::string(stringSumHelper&& rval) {
  init();
  move(rval);
}
#endif

string::string(char c) {
  init();
  char buf[2]{c,0};
  operator=(buf);
}

string::string(unsigned char value, uint8_t base) {
  init();
  char buf[1 + 8 * sizeof(unsigned char)];
  utoa(value, buf, base);
  operator=(buf);
}

string::string(int value, uint8_t base) {
  init();
  char buf[2 + 8 * sizeof(int)];
  itoa(value, buf, base);
  operator=(buf);
}

string::string(unsigned int value, uint8_t base) {
  init();
  char buf[1 + 8 * sizeof(unsigned int)];
  utoa(value, buf, base);
  operator=(buf);
}

string::string(long value, uint8_t base) {
  init();
  char buf[2 + 8 * sizeof(long)];
  ltoa(value, buf, base);
  operator=(buf);
}

string::string(unsigned long value, uint8_t base) {
  init();
  char buf[1 + 8 * sizeof(unsigned long)];
  ultoa(value, buf, base);
  operator=(buf);
}

string::string(float value, uint8_t decimalPlaces) {
  init();
  char buf[33];
  operator=(dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf));
}

string::string(double value, uint8_t decimalPlaces) {
  init();
  char buf[33];
  operator=(dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf));
}

string::~string() {
  if(buffer) free(buffer);
}

inline void string::init(void) {
  buffer = NULL;
  capacity = 0;
  len = 0;
}

void string::invalidate(void) {
  if(buffer) free(buffer);
  buffer = NULL;
  capacity = len = 0;
}

unsigned char string::reserve(unsigned int size) {
  if(buffer && capacity >= size) return 1;
  if(changeBuffer(size)) {
    if(len == 0) buffer[0] = 0;
    return 1;
  }
  return 0;
}

unsigned char string::changeBuffer(unsigned int maxStrLen) {
  char* newbuffer = reinterpret_cast<char*>(realloc(buffer, maxStrLen + 1));
  if(newbuffer) {
    buffer = newbuffer;
    capacity = maxStrLen;
    return 1;
  }
  return 0;
}

string& string::copy(const char* cstr, unsigned int length) {
  if(!reserve(length)) {
    invalidate();
    return *this;
  }
  len = length;
  strcpy(buffer, cstr);
  return *this;
}

string& string::copy(const __FlashStringHelper* fstr, unsigned int length) {
  if(!reserve(length)) {
    invalidate();
    return *this;
  }
  len = length;
  strcpy_P(buffer, (PGM_P)fstr);
  return *this;
}

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
void string::move(string& rhs) {
  if(buffer) {
    if(rhs && capacity >= rhs.len) {
      strcpy(buffer, rhs.buffer);
      len = rhs.len;
      rhs.len = 0;
      return;
    }
    else {
      free(buffer);
    }
  }
  buffer = rhs.buffer;
  capacity = rhs.capacity;
  len = rhs.len;
  rhs.buffer = NULL;
  rhs.capacity = 0;
  rhs.len = 0;
}
#endif

string& string::operator=(const string& rhs) {
  if(this == &rhs) return *this;

  if(rhs.buffer) copy(rhs.buffer, rhs.len);
  else invalidate();

  return *this;
  }

string& string::operator=(const String& str) {
  if(str.begin()) copy(str.begin(), str.length());
  else invalidate();
  return *this;
}

#if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__)
string& string::operator=(string&& rval) {
  if(this != &rval) move(rval);
  return *this;
}

string& string::operator=(stringSumHelper&& rval) {
  if(this != &rval) move(rval);
  return *this;
}
#endif

string& string::operator=(const char* cstr) {
  if(cstr) copy(cstr, strlen(cstr));
  else invalidate();

  return *this;
}

string& string::operator=(const __FlashStringHelper* fstr) {
  if(fstr) copy(fstr, strlen_P((PGM_P)fstr));
  else invalidate();

  return *this;
}

uint8_t string::concat(const string& s) {
  return concat(s.buffer, s.len);
}

uint8_t string::concat(const String& str) {
  return concat(str.begin(), str.length());
}

uint8_t string::concat(const char* cstr, unsigned int length) {
  unsigned int newlen = len + length;
  if(!cstr) return 0;
  if(length == 0) return 1;
  if(!reserve(newlen)) return 0;
  strcpy(buffer + len, cstr);
  len = newlen;
  return 1;
}

uint8_t string::concat(const char* cstr) {
  if(!cstr) return 0;
  return concat(cstr, strlen(cstr));
}

uint8_t string::concat(char c) {
  char buf[2]{c, 0};
  return concat(buf, 1);
}

uint8_t string::concat(int num) {
  char buf[2 + 3 * sizeof(int)];
  itoa(num, buf, 10);
  return concat(buf, strlen(buf));
}

uint8_t string::concat(long num) {
  char buf[2 + 3 * sizeof(long)];
  ltoa(num, buf, 10);
  return concat(buf, strlen(buf));
}

uint8_t string::concat(unsigned char num) {
  char buf[1 + 3 * sizeof(unsigned char)];
  itoa(num, buf, 10);
  return concat(buf, strlen(buf));
}

uint8_t string::concat(unsigned int num) {
  char buf[1 + 3 * sizeof(unsigned int)];
  utoa(num, buf, 10);
  return concat(buf, strlen(buf));
}

uint8_t string::concat(unsigned long num) {
  char buf[1 + 3 * sizeof(unsigned long)];
  ultoa(num, buf, 10);
  return concat(buf, strlen(buf));
}

uint8_t string::concat(float num) {
  char buf[20];
  char* string = dtostrf(num, 4, 2, buf);
  return concat(string, strlen(string));
}

uint8_t string::concat(double num) {
  char buf[20];
  char* string = dtostrf(num, 4, 2, buf);
  return concat(string, strlen(string));
}

uint8_t string::concat(const __FlashStringHelper* str) {
  if(!str) return 0;
  int length = strlen_P((const char*)str);
  if(length == 0) return 1;
  unsigned int newlen = len + length;
  if(!reserve(newlen)) return 0;
  strcpy_P(buffer + len, (const char*)str);
  len = newlen;
  return 1;
}

stringSumHelper& operator+(const stringSumHelper& lhs, const string& rhs) {
  stringSumHelper& a = const_cast<stringSumHelper&>(lhs);
  if(!a.concat(rhs.buffer, rhs.len)) a.invalidate();
  return a;
}

stringSumHelper& operator+(const stringSumHelper& lhs, const String& rhs) {
  stringSumHelper& a = const_cast<stringSumHelper&>(lhs);
  if(!a.concat(rhs.begin(), rhs.length())) a.invalidate();
  return a;
}

stringSumHelper& operator+(const stringSumHelper& lhs, const char* cstr) {
  stringSumHelper& a = const_cast<stringSumHelper&>(lhs);
  if(!cstr || !a.concat(cstr, strlen(cstr))) a.invalidate();
  return a;
}

stringSumHelper& operator+(const stringSumHelper& lhs, char c) {
  stringSumHelper& a = const_cast<stringSumHelper&>(lhs);
  if(!a.concat(c)) a.invalidate();
  return a;
}

stringSumHelper& operator+(const stringSumHelper& lhs, unsigned char num) {
  stringSumHelper& a = const_cast<stringSumHelper&>(lhs);
  if(!a.concat(num)) a.invalidate();
  return a;
}

stringSumHelper& operator+(const stringSumHelper& lhs, int num) {
  stringSumHelper& a = const_cast<stringSumHelper&>(lhs);
  if(!a.concat(num)) a.invalidate();
  return a;
}

stringSumHelper& operator+(const stringSumHelper& lhs, unsigned int num) {
  stringSumHelper& a = const_cast<stringSumHelper&>(lhs);
  if(!a.concat(num)) a.invalidate();
  return a;
}

stringSumHelper& operator+(const stringSumHelper& lhs, long num) {
  stringSumHelper& a = const_cast<stringSumHelper&>(lhs);
  if(!a.concat(num)) a.invalidate();
  return a;
}

stringSumHelper& operator+(const stringSumHelper& lhs, unsigned long num) {
  stringSumHelper& a = const_cast<stringSumHelper&>(lhs);
  if(!a.concat(num)) a.invalidate();
  return a;
}

stringSumHelper& operator+(const stringSumHelper& lhs, float num) {
  stringSumHelper& a = const_cast<stringSumHelper&>(lhs);
  if(!a.concat(num)) a.invalidate();
  return a;
}

stringSumHelper& operator+(const stringSumHelper& lhs, double num) {
  stringSumHelper& a = const_cast<stringSumHelper&>(lhs);
  if(!a.concat(num)) a.invalidate();
  return a;
}

stringSumHelper& operator+(const stringSumHelper& lhs, const __FlashStringHelper* rhs) {
  stringSumHelper& a = const_cast<stringSumHelper&>(lhs);
  if(!a.concat(rhs))	a.invalidate();
  return a;
}

int string::compareTo(const string& s) const {
  if(!buffer || !s.buffer) {
    if(s.buffer && s.len > 0) return 0 - *(unsigned char*)s.buffer;
    if(buffer && len > 0) return *(unsigned char*)buffer;
    return 0;
  }
  return strcmp(buffer, s.buffer);
}

bool string::equals(const string& s) const {
  return (len == s.len && compareTo(s) == 0);
}

bool string::equals(const char* cstr) const {
  if(len == 0) return (cstr == NULL || *cstr == 0);
  if(cstr == NULL) return buffer[0] == 0;
  return strcmp(buffer, cstr) == 0;
}

bool string::operator==(const char ch) const {
  if(buffer && len == 1)
    return buffer[0] == ch;
  return false;
}

bool string::operator<(const string& rhs) const {
  return compareTo(rhs) < 0;
}

bool string::operator>(const string& rhs) const {
  return compareTo(rhs) > 0;
}

bool string::operator<=(const string& rhs) const {
  return compareTo(rhs) <= 0;
}

bool string::operator>=(const string& rhs) const {
  return compareTo(rhs) >= 0;
}

uint8_t string::equalsIgnoreCase(const string& s2) const {
  if(this == &s2) return 1;
  if(len != s2.len) return 0;
  if(len == 0) return 1;
  const char* p1 = buffer;
  const char* p2 = s2.buffer;
  while(*p1) {
    if(tolower(*p1++) != tolower(*p2++)) return 0;
  }
  return 1;
}

bool string::startsWith(const char c) const {
  if(buffer)
    return buffer[0] == c;
  return false;
}

bool string::startsWith(const string& s) const {
  if(len < s.len) return 0;
  return startsWith(s, 0);
}

bool string::startsWith(const string& s, unsigned int offset) const {
  if(offset > len - s.len || !buffer || !s.buffer) return 0;
  return strncmp(&buffer[offset], s.buffer, s.len) == 0;
}

bool string::endsWith(const char c) const {
  if(buffer)
    return buffer[len - 1] == c;
  return false;
}

bool string::endsWith(const string& s2) const {
  if(len < s2.len || !buffer || !s2.buffer) return 0;
  return strcmp(&buffer[len - s2.len], s2.buffer) == 0;
}

char string::charAt(unsigned int loc) const {
  return operator[](loc);
}

void string::setCharAt(unsigned int loc, char c) {
  if(loc < len) buffer[loc] = c;
}

char& string::operator[](unsigned int index) {
  static char dummy_writable_char;
  if(index >= len || !buffer) {
    dummy_writable_char = 0;
    return dummy_writable_char;
  }
  return buffer[index];
}

char string::operator[](unsigned int index) const {
  if(index >= len || !buffer) return 0;
  return buffer[index];
}

void string::getBytes(unsigned char* buf, unsigned int bufsize, unsigned int index) const {
  if(!bufsize || !buf) return;
  if(index >= len) {
    buf[0] = 0;
    return;
  }
  unsigned int n = bufsize - 1;
  if(n > len - index) n = len - index;
  strncpy((char*)buf, buffer + index, n);
  buf[n] = 0;
}

void string::toCharArray(char* buf, unsigned int bufsize, unsigned int index) const {
  getBytes((uint8_t*)buf, bufsize, index);
}

int string::indexOf(char c) const {
  return indexOf(c, 0);
}

int string::indexOf(char ch, unsigned int fromIndex) const {
  if(fromIndex >= len) return -1;
  const char* temp = strchr(buffer + fromIndex, ch);
  if(temp == NULL) return -1;
  return temp - buffer;
}

int string::indexOf(const string& s2) const {
  return indexOf(s2, 0);
}

int string::indexOf(const string& s2, unsigned int fromIndex) const {
  if(fromIndex >= len) return -1;
  const char* found = strstr(buffer + fromIndex, s2.buffer);
  if(found == NULL) return -1;
  return found - buffer;
}

int string::lastIndexOf(char theChar) const {
  return lastIndexOf(theChar, len - 1);
}

int string::lastIndexOf(char ch, unsigned int fromIndex) const {
  if(fromIndex >= len) return -1;
  char tempchar = buffer[fromIndex + 1];
  buffer[fromIndex + 1] = 0;
  char* temp = strrchr(buffer, ch);
  buffer[fromIndex + 1] = tempchar;
  if(temp == NULL) return -1;
  return temp - buffer;
}

int string::lastIndexOf(const string& s) const {
  return lastIndexOf(s, len - s.len);
}

int string::lastIndexOf(const string& s, unsigned int fromIndex) const {
  if(s.len == 0 || len == 0 || s.len > len) return -1;
  if(fromIndex >= len) fromIndex = len - 1;
  int found = -1;
  for(char* p = buffer; p <= buffer + fromIndex; p++) {
    p = strstr(p, s.buffer);
    if(!p) break;
    if((unsigned int)(p - buffer) <= fromIndex) found = p - buffer;
  }
  return found;
}

string string::substring(unsigned int left, unsigned int right) const {
  if(left > right) {
    unsigned int temp = right;
    right = left;
    left = temp;
  }
  string out;
  if(left >= len) return out;
  if(right > len) right = len;
  char temp = buffer[right];  // save the replaced character
  buffer[right] = 0;
  out = buffer + left;  // pointer arithmetic
  buffer[right] = temp;  //restore character
  return out;
}

void string::replace(char find, char replace) {
  if(!buffer) return;
  for(char* p = buffer; *p; p++) {
    if(*p == find) *p = replace;
  }
}

void string::replace(const string& find, const string& replace) {
  if(len == 0 || find.len == 0) return;
  int diff = replace.len - find.len;
  char* readFrom = buffer;
  char* foundAt;
  if(diff == 0) {
    while((foundAt = strstr(readFrom, find.buffer)) != NULL) {
      memcpy(foundAt, replace.buffer, replace.len);
      readFrom = foundAt + replace.len;
    }
  }
  else if(diff < 0) {
    char* writeTo = buffer;
    while((foundAt = strstr(readFrom, find.buffer)) != NULL) {
      unsigned int n = foundAt - readFrom;
      memcpy(writeTo, readFrom, n);
      writeTo += n;
      memcpy(writeTo, replace.buffer, replace.len);
      writeTo += replace.len;
      readFrom = foundAt + find.len;
      len += diff;
    }
    strcpy(writeTo, readFrom);
  }
  else {
    unsigned int size = len; // compute size needed for result
    while((foundAt = strstr(readFrom, find.buffer)) != NULL) {
      readFrom = foundAt + find.len;
      size += diff;
    }
    if(size == len) return;
    if(size > capacity && !changeBuffer(size)) return; // XXX: tell user!
    int index = len - 1;
    while(index >= 0 && (index = lastIndexOf(find, index)) >= 0) {
      readFrom = buffer + index + find.len;
      memmove(readFrom + diff, readFrom, len - (readFrom - buffer));
      len += diff;
      buffer[len] = 0;
      memcpy(buffer + index, replace.buffer, replace.len);
      index--;
    }
  }
}

void string::remove(unsigned int index) {
  // Pass the biggest integer as the count. The remove method
  // below will take care of truncating it at the end of the
  // string.
  remove(index, (unsigned int)-1);
}

void string::remove(unsigned int index, unsigned int count) {
  if(index >= len) { return; }
  if(count <= 0) { return; }
  if(count > len - index) { count = len - index; }
  char* writeTo = buffer + index;
  len = len - count;
  strncpy(writeTo, buffer + index + count, len - index);
  buffer[len] = 0;
}

void string::toLowerCase(void) {
  if(!buffer) return;
  for(char* p = buffer; *p; p++) {
    *p = tolower(*p);
  }
}

void string::toUpperCase(void) {
  if(!buffer) return;
  for(char* p = buffer; *p; p++) {
    *p = toupper(*p);
  }
}

void string::trim(void) {
  if(!buffer || len == 0) return;
  char* begin = buffer;
  while(isspace(*begin)) begin++;
  char* end = buffer + len - 1;
  while(isspace(*end) && end >= begin) end--;
  len = end + 1 - begin;
  if(begin > buffer) memcpy(buffer, begin, len);
  buffer[len] = 0;
}

/*********************************************/
/*  Parsing / Conversion                     */
/*********************************************/



int string::toInt(void) const {
  if(buffer) return atoi(buffer);
  return 0;
}

long string::toLong(void) const {
  if(buffer) return atol(buffer);
  return 0;
}

float string::toFloat(void) const {
  return float(toDouble());
}

double string::toDouble(void) const {
  if(buffer) return atof(buffer);
  return 0;
}