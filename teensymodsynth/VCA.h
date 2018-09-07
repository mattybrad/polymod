#ifndef VCA_h
#define VCA_h
#include "Arduino.h"
#include "Module.h"
#include <Audio.h>

class VCA : public Module {
  public:
    VCA();
    virtual void update();
  private:
    AudioMixer4 _cvMixer;
    AudioMixer4 _inputMixer;
    AudioEffectMultiply _multiplier;
    AudioConnection* _patchCable1;
    AudioConnection* _patchCable2;
};

#endif
