#pragma once

#include "firmware.h"

#if EXPLORER

#include "string.hpp"

class Explorer {
  string cwd = F("/");
public:

  Explorer();

  ~Explorer();

  void printSdInfo();

  void list();

  bool mkdir(const string& path);

  bool rmdir(const string& path);

  //0 - Successful
  //1 - Driver error
  //2 - No such directory
  //3 - Not a directory
  int chdir(const string& path);

  bool remove(const string& path);

  //0 - Successful
  //1 - File not found
  //2 - Target file is a directory
  int move(const string& fromPath, const string& toPath);

  //0 - Successful
  //1 - Driver error
  int touch(const string& path);

  //0 - Successful
  //1 - Driver error
  //2 - Is a directory
  //3 - File not found
  int cat(const string& path);

  //0 - Successful
  //1 - Driver error
  //2 - Is a directory
  int tee(const string& path, const string& data);

  void edit(const string& path);

  void uname(); 
  
  void init();

  void interactive();
};
extern Explorer* explorer;
#endif