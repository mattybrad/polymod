#ifndef Mixer_h
#define Mixer_h
#include "Arduino.h"
#include "Module.h"
#include <Audio.h>

class Mixer : public Module {
  public:
    Mixer();
  private:
    AudioMixer4 _mixer;
};

#endif
