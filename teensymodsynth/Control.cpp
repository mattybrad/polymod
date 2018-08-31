#include "Arduino.h"
#include "Control.h"

Control::Control() {
  
}

void Control::updateSmoothedValue() {
  float rawValueFloat = ((float) rawValue) / 1023.0;
  _smoothedValue = rawValueFloat; // temp
}

float Control::getSmoothedValue() {
  return _smoothedValue;
}

