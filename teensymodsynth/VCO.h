#ifndef VCO_h
#define VCO_h
#include "Arduino.h"
#include "Module.h"
#include <Audio.h>

class VCO : public Module {
  public:
    VCO();
  private:
    AudioSynthWaveform _saw;
    AudioSynthWaveform _square;
    AudioSynthWaveform _triangle;
    AudioSynthWaveform _sine;
};

#endif
