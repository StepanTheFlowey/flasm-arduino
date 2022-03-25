#include "explorer.hpp"

#if EXPLORER

#include "hardware.h"
#include "switch.h"

Explorer* explorer = nullptr;

Explorer::Explorer() {
  debug(F("Explorer()"));
}

Explorer::~Explorer() {
  debug(F("~Explorer()"));
}

void Explorer::printSdInfo() {
  if(sd.exists(String('/'))) {
    Serial.print(F("Format: FAT"));
    Serial.println(sd.vol()->fatType());
  }
  else {
    Serial.println(F("SD card ejected"));
  }
}

void Explorer::list() {
  sd.chdir(cwd.c_str());
  File vwd = *sd.vwd(); //Copying file data
  File file;
  while(file.openNext(&vwd, O_READ)) {
    file.printModifyDateTime(&Serial);
    Serial.print(file.isDir() ? F(" <DIR>  ") : F(" <FILE> "));
    file.printName(&Serial);
    Serial.println();
    file.close();
  }
}

bool Explorer::mkdir(const string& path) {
  return sd.mkdir((cwd + '/' + path).c_str());
}

bool Explorer::rmdir(const string& path) {
  return sd.rmdir((cwd + '/' + path).c_str());
}

int Explorer::chdir(const string& path) {
  //Change work dir to root
  if(path.length() == 0) {
    cwd = '/';
    return sd.chdir() ? 0 : 1;
  }

  //Go back
  if(path == F("..")) {
    uint16_t index = cwd.lastIndexOf('/');
    if(index == 0) {
      cwd = '/';
    }
    else {
      cwd[index] = '\0';
    }
    return sd.chdir(cwd.c_str()) ? 0 : 1;
  }

  if(!sd.exists(path.c_str())) {
    return 2;
  }

  File dir(path.c_str(), O_RDONLY);
  if(!dir.isDir()) {
    return 3;
  }
  if(path.startsWith('/')) {
    cwd = path;
  }
  else {
    if(cwd == '/') {
      cwd += path;
    }
    else {
      cwd += '/' + path;
    }
  }

  if(cwd.endsWith('/')) {
    *cwd.end() = '\0';
  }
  return sd.chdir(cwd.c_str()) ? 0 : 1;
}

bool Explorer::remove(const string& path) {
  return sd.remove((cwd + '/' + path).c_str());
}

int Explorer::move(const string& fromPath, const string& toPath) {
  Serial.println(F("Not implemented yet"));
  return false;
}

int Explorer::touch(const string& path) {
  File file(path.c_str(), O_WRONLY | O_CREAT);
  return file ? 0 : 1;
}

int Explorer::cat(const string& path) {
  if(!sd.exists(path.c_str())) {
    return 3;
  }
  File file(path.c_str(), O_RDONLY);
  if(!file) {
    return 1;
  }
  if(file.isDir()) {
    return 2;
  }
  while(file.available32()) {
    Serial.print(static_cast<char>(file.read()));
  }
  Serial.println();
  Serial.flush();
  return 0;
}

//0 - Successful
//1 - Driver error
//2 - Is a directory

int Explorer::tee(const string& path, const string& data) {
  File file(path.c_str(), O_WRONLY | O_CREAT | O_APPEND);
  if(!file) {
    return 1;
  }
  if(file.isDir()) {
    return 2;
  }
  file.write(data.c_str());
  return 0;
}

void Explorer::edit(const string& path) {
  Serial.println(F("Not implemented yet"));
}

void Explorer::uname() {
  Serial.println(F("Target hardware info:"));
  Serial.print(F("Board name: "));
  Serial.println(BOARD_NAME);
  Serial.print(F("MCU name: "));
  Serial.println(MCU_NAME);
  Serial.print(F("MCU freq: "));
  Serial.println(F_CPU);
  Serial.print(F("Arch name: "));
  Serial.println(ARCH_NAME);
}

void Explorer::init() {
  Serial.print(F("Initializing SD card... "));
  if(sd.begin(PIN_SD_CS)) {
    Serial.println(F("Done"));
  }
  else {
    Serial.println(F("Error"));
  }
}

void Explorer::interactive() {
  printSdInfo();
  list();
  string com;
  while(true) {
    Serial.print(cwd);
    Serial.print(F("$ "));
    stateLedOff();
    while(!Serial.available()) {}
    stateLedOn();
    com = Serial.readStringUntil(' ');
    Serial.println();

    switch(strHashSwitch(com.c_str())) {
      case strHashSwitch("exit"):
        Serial.println(F("exit"));
        return;

      case strHashSwitch("dir"):
      case strHashSwitch("ls"):
        list();
        break;

      case strHashSwitch("chdir"):
      case strHashSwitch("cd"):
        switch(chdir(Serial.readStringUntil(' '))) {
          case 0:
            Serial.println(F("Done"));
            break;
          case 1:
            Serial.println(F("Driver error"));
            break;
          case 2:
            Serial.println(F("No such directory"));
            break;
          case 3:
            Serial.println(F("Not a directory"));
            break;
        }
        break;

      case strHashSwitch("mkdir"):
      case strHashSwitch("md"):
        if(mkdir(Serial.readStringUntil(' ')))
          Serial.println(F("Done"));
        else
          Serial.println(F("Driver error"));
        break;

      case strHashSwitch("rmdir"):
      case strHashSwitch("rd"):
        if(rmdir(Serial.readStringUntil(' ')))
          Serial.println(F("Done"));
        else
          Serial.println(F("Driver error"));
        break;

      case strHashSwitch("touch"):
        switch(touch(Serial.readStringUntil(' '))) {
          case 0:
            Serial.println(F("Done"));
            break;
          case 1:
            Serial.println(F("Driver error"));
            break;
        }
        break;

      case strHashSwitch("cat"):
        switch(cat(Serial.readStringUntil(' '))) {
          case 0:
            Serial.println(F("Done"));
            break;
          case 1:
            Serial.println(F("Driver error"));
            break;
          case 2:
            Serial.println(F("Is a directory"));
            break;
          case 3:
            Serial.println(F("File not found"));
            break;
        }
        break;

      case strHashSwitch("tee"):
        switch(tee(Serial.readStringUntil(' '), Serial.readStringUntil(' '))) {
          case 0:
            Serial.println(F("Done"));
            break;
          case 1:
            Serial.println(F("Driver error"));
            break;
          case 2:
            Serial.println(F("Is a directory"));
            break;
        }
        break;

      case strHashSwitch("remove"):
      case strHashSwitch("rm"):
        if(remove(Serial.readStringUntil(' ')))
          Serial.println(F("Done"));
        else
          Serial.println(F("Driver error"));
        break;

      case strHashSwitch("move"):
      case strHashSwitch("mv"):
        move(Serial.readStringUntil(' '), Serial.readStringUntil(' '));
        break;

      case strHashSwitch("edit"):
        edit(Serial.readStringUntil(' '));
        break;

      case strHashSwitch("init"):
        init();
        break;

      case strHashSwitch("eject"):
        sd.end();
        cwd = '/';
        Serial.println(F("Done"));
        break;

      case strHashSwitch("uname"):
        uname();
        break;

      default:
        Serial.println(F("Unknown command"));
        break;
    }
  }
}

#endif // EXPLORER