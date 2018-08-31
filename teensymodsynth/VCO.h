#ifndef VCO_h
#define VCO_h
#include "Arduino.h"
#include "Module.h"
#include <Audio.h>

class VCO : public Module {
  public:
    VCO();
    virtual void update();
  private:
    AudioSynthWaveformModulated _saw;
    AudioSynthWaveformModulated _square;
    AudioSynthWaveformModulated _triangle;
    AudioSynthWaveformModulated _sine;
    AudioMixer4 _freqMod;
    AudioConnection* _patchCable1;
    AudioConnection* _patchCable2;
    AudioConnection* _patchCable3;
    AudioConnection* _patchCable4;
};

#endif
