#ifndef LFO_h
#define LFO_h
#include "Arduino.h"
#include "Module.h"
#include <Audio.h>

class LFO : public Module {
  public:
    LFO();
    virtual void update();
  private:
    AudioSynthWaveformModulated _saw;
    AudioSynthWaveformModulated _inverseSaw;
    AudioSynthWaveformModulated _sine;
    AudioSynthWaveformModulated _triangle;
    AudioSynthWaveformModulated _square;
    AudioAmplifier _freqMod;
    AudioConnection* _patchCable1;
    AudioConnection* _patchCable2;
    AudioConnection* _patchCable3;
    AudioConnection* _patchCable4;
    AudioConnection* _patchCable5;
};

#endif
