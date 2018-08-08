#ifndef Envelope_h
#define Envelope_h
#include "Arduino.h"
#include "Module.h"
#include <Audio.h>

class Envelope : public Module {
  public:
    Envelope();
  private:
    AudioEffectEnvelope _envelope;
};

#endif
