#include "Arduino.h"
#include "Module.h"

Module::Module() {
  _id = 1;
}

int Module::getID() {
  return _id;
}

Socket& Module::getSocket(int moduleSocketNumber) {
  return sockets[moduleSocketNumber];
}

void Module::removePatchCable(int moduleSocketNumber) {
  
}

void Module::updateControlValue(int controlIndex, int controlReading) {
  controls[controlIndex].rawValue = controlReading;
  controls[controlIndex].updateSmoothedValue();
}

