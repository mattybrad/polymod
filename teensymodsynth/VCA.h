#ifndef VCA_h
#define VCA_h
#include "Arduino.h"
#include "Module.h"
#include <Audio.h>

class VCA : public Module {
  public:
    VCA();
  private:
    AudioEffectMultiply _vca;
};

#endif
