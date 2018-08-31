#ifndef Control_h
#define Control_h
#include "Arduino.h"

class Control {
  public:
    Control();
    int rawValue;
    void updateSmoothedValue();
    float getSmoothedValue();
  private:
    float _smoothedValue = 0.0;
};

#endif
