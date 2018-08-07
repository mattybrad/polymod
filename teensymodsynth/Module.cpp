#include "Arduino.h"
#include "Module.h"

#define OUTPUT_MODULE 1
#define VCO_MODULE 2
#define VCF_MODULE 3
#define VCA_MODULE 4

Module::Module() {
  _id = 1;
}

int Module::getID() {
  return _id;
}

Socket& Module::getSocket(int moduleSocketNumber) {
  return sockets[moduleSocketNumber];
}

