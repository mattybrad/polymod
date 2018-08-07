#include "Arduino.h"
#include "Module.h"

#define OUTPUT_MODULE 1
#define VCO_MODULE 2
#define VCF_MODULE 3
#define VCA_MODULE 4

Module::Module(int moduleID) {
  _id = moduleID;
}

int Module::getID() {
  return _id;
}

