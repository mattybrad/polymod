#include "Arduino.h"
#include "Module.h"

Module::Module(int moduleID) {
  _id = moduleID;
  
  // initialise module based on its type (VCO, VCF, LFO, etc)
  switch(_id) {
    case 1:
    Serial.println("VCO");
    break;
    
    case 2:
    Serial.println("VCF");
    break;

    default:
    Serial.println("unrecognised ID");
  }
}

int Module::getID() {
  return _id;
}

