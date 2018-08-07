#ifndef LFO_h
#define LFO_h
#include "Arduino.h"
#include "Module.h"
#include <Audio.h>

class LFO : public Module {
  public:
    LFO();
  private:
    AudioSynthWaveform _saw;
    AudioSynthWaveform _inverseSaw;
    AudioSynthWaveform _sine;
    AudioSynthWaveform _triangle;
    AudioSynthWaveform _square;
};

#endif
